#include "adapters/ui/gtk_touch_adapter.hpp"
#include <algorithm>
#include <iostream>

namespace gach::adapters::ui {

GtkTouchAdapter::GtkTouchAdapter(application::StoryInteractor& interactor, CairoRenderer& renderer)
    : m_interactor(interactor), m_renderer(renderer) {}

void GtkTouchAdapter::initialize(int argc, char** argv, int width, int height) {
    gtk_init(&argc, &argv);

    m_virtual_w = (width > 0) ? width : 1072;
    m_virtual_h = (height > 0) ? height : 1448;

    m_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

#if !GTK_CHECK_VERSION(3, 0, 0)
    // Tương thích Kindle OS Window Manager (GTK 2.0): Toàn màn hình, không viền, ẩn Status Bar (PC:N = Pillow Chrome None)
    gtk_window_set_title(GTK_WINDOW(m_window), "L:A_N:application_ID:com.trongnghiango.gach_PC:N");
    gtk_window_fullscreen(GTK_WINDOW(m_window));
    gtk_window_set_decorated(GTK_WINDOW(m_window), FALSE);
#else
    // Môi trường Desktop Simulator (GTK 3.0): Tỷ lệ chuẩn PW3 (1072:1448), vừa vặn màn hình Full HD
    int default_win_h = 880;
    int default_win_w = static_cast<int>(default_win_h * (static_cast<double>(m_virtual_w) / m_virtual_h));
    gtk_window_set_title(GTK_WINDOW(m_window), "GACH — Kindle Paperwhite 3 Simulator (1072x1448)");
    gtk_window_set_default_size(GTK_WINDOW(m_window), default_win_w, default_win_h);
#endif

    g_signal_connect(m_window, "destroy", G_CALLBACK(gtk_main_quit), nullptr);

    m_drawing_area = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(m_window), m_drawing_area);

    gtk_widget_add_events(m_drawing_area, GDK_BUTTON_PRESS_MASK);

#if GTK_CHECK_VERSION(3, 0, 0)
    g_signal_connect(m_drawing_area, "draw", G_CALLBACK(on_draw_event), this);
#else
    g_signal_connect(m_drawing_area, "expose-event", G_CALLBACK(on_expose_event), this);
#endif
    g_signal_connect(m_drawing_area, "button-press-event", G_CALLBACK(on_button_press_event), this);
    g_signal_connect(m_window, "key-press-event", G_CALLBACK(on_key_press_event), this);

    // Renderer luôn tính toán bố cục ở độ phân giải ảo chuẩn PW3
    m_renderer.set_resolution(m_virtual_w, m_virtual_h);

    m_timer_id = g_timeout_add(100, on_timer_tick_event, this);

    gtk_widget_show_all(m_window);
}

void GtkTouchAdapter::run() {
    gtk_main();
}

#if GTK_CHECK_VERSION(3, 0, 0)
gboolean GtkTouchAdapter::on_draw_event(GtkWidget* widget, cairo_t* cr, gpointer user_data) {
    auto* self = static_cast<GtkTouchAdapter*>(user_data);
    if (!self) return FALSE;

    int win_w = gtk_widget_get_allocated_width(widget);
    int win_h = gtk_widget_get_allocated_height(widget);

    double sx = static_cast<double>(win_w) / self->m_virtual_w;
    double sy = static_cast<double>(win_h) / self->m_virtual_h;
    self->m_scale = std::min(sx, sy);

    double draw_w = self->m_virtual_w * self->m_scale;
    double draw_h = self->m_virtual_h * self->m_scale;
    self->m_offset_x = (win_w - draw_w) / 2.0;
    self->m_offset_y = (win_h - draw_h) / 2.0;

    // Nền tối thanh lịch giả lập viền máy Kindle (Kindle Bezel)
    cairo_set_source_rgb(cr, 0.15, 0.15, 0.15);
    cairo_paint(cr);

    cairo_save(cr);
    cairo_translate(cr, self->m_offset_x, self->m_offset_y);
    cairo_scale(cr, self->m_scale, self->m_scale);

    self->m_renderer.draw_to_context(cr);
    cairo_restore(cr);

    return FALSE;
}
#else
gboolean GtkTouchAdapter::on_expose_event(GtkWidget* widget, [[maybe_unused]] GdkEventExpose* event, gpointer user_data) {
    auto* self = static_cast<GtkTouchAdapter*>(user_data);
    if (!self) return FALSE;

#if GTK_CHECK_VERSION(2, 14, 0)
    GdkWindow* win = gtk_widget_get_window(widget);
#else
    GdkWindow* win = widget->window;
#endif
    cairo_t* cr = gdk_cairo_create(win);
    self->m_renderer.draw_to_context(cr);
    cairo_destroy(cr);

    return FALSE;
}
#endif

gboolean GtkTouchAdapter::on_button_press_event([[maybe_unused]] GtkWidget* widget, GdkEventButton* event, gpointer user_data) {
    auto* self = static_cast<GtkTouchAdapter*>(user_data);
    if (!self || !event) return FALSE;

    guint32 now = event->time;
    if (now - self->m_last_click_time < 300) {
        return TRUE;
    }
    self->m_last_click_time = now;

    // Ánh xạ tọa độ click chuột trên cửa sổ thành tọa độ cảm ứng chuẩn PW3
    double virt_x = (event->x - self->m_offset_x) / self->m_scale;
    double virt_y = (event->y - self->m_offset_y) / self->m_scale;

    if (virt_x >= 0 && virt_x <= self->m_virtual_w &&
        virt_y >= 0 && virt_y <= self->m_virtual_h) {
        // Kiểm tra nút Thoát ứng dụng (chỉ khi không có overlay nào mở)
        if (self->m_interactor.overlay_mode() == application::OverlayMode::NONE &&
            self->m_interactor.layout().hit_test_exit(static_cast<int>(virt_x), static_cast<int>(virt_y))) {
            std::cout << "[INFO] Exit button touched. Exiting GACH cleanly..." << std::endl;
            gtk_main_quit();
            return TRUE;
        }

        self->m_interactor.on_touch(static_cast<int>(virt_x), static_cast<int>(virt_y));
        gtk_widget_queue_draw(self->m_drawing_area);
    }
    return TRUE;
}

gboolean GtkTouchAdapter::on_key_press_event([[maybe_unused]] GtkWidget* widget, GdkEventKey* event, gpointer user_data) {
    if (!event) return FALSE;
    auto* self = static_cast<GtkTouchAdapter*>(user_data);
    // Phím Escape hoặc Q để thoát hoặc đóng overlay
    if (event->keyval == 0xff1b || event->keyval == 'q' || event->keyval == 'Q') {
        if (self && self->m_interactor.overlay_mode() != application::OverlayMode::NONE) {
            std::cout << "[INFO] Escape pressed. Closing overlay..." << std::endl;
            self->m_interactor.close_overlay();
            gtk_widget_queue_draw(self->m_drawing_area);
            return TRUE;
        }
        std::cout << "[INFO] Quit key pressed. Exiting GACH cleanly..." << std::endl;
        gtk_main_quit();
        return TRUE;
    }
    return FALSE;
}

gboolean GtkTouchAdapter::on_timer_tick_event(gpointer user_data) {
    auto* self = static_cast<GtkTouchAdapter*>(user_data);
    if (self) {
        self->m_interactor.on_timer_tick(100);
        gtk_widget_queue_draw(self->m_drawing_area);
    }
    return TRUE;
}

} // namespace gach::adapters::ui
