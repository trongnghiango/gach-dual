#include "adapters/ui/gtk_touch_adapter.hpp"
#include <iostream>

namespace gach::adapters::ui {

GtkTouchAdapter::GtkTouchAdapter(application::StoryInteractor& interactor, CairoRenderer& renderer)
    : m_interactor(interactor), m_renderer(renderer) {}

void GtkTouchAdapter::initialize(int argc, char** argv, int width, int height) {
    gtk_init(&argc, &argv);

    m_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

#if !GTK_CHECK_VERSION(3, 0, 0)
    // Tương thích Kindle OS Window Manager (GTK 2.0): Toàn màn hình, không viền
    gtk_window_set_title(GTK_WINDOW(m_window), "L:A_N:application_ID:com.trongnghiango.gach_PC:T");
    gtk_window_fullscreen(GTK_WINDOW(m_window));
    gtk_window_set_decorated(GTK_WINDOW(m_window), FALSE);
#else
    // Môi trường Desktop Dev (GTK 3.0)
    gtk_window_set_title(GTK_WINDOW(m_window), "GACH - Kindle-Mít Story Phonics");
    gtk_window_set_default_size(GTK_WINDOW(m_window), width, height);
#endif

    g_signal_connect(m_window, "destroy", G_CALLBACK(gtk_main_quit), nullptr);

    m_drawing_area = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(m_window), m_drawing_area);

    // Bắt sự kiện chạm cảm ứng và chuột
    gtk_widget_add_events(m_drawing_area, GDK_BUTTON_PRESS_MASK);

#if GTK_CHECK_VERSION(3, 0, 0)
    g_signal_connect(m_drawing_area, "draw", G_CALLBACK(on_draw_event), this);
#else
    g_signal_connect(m_drawing_area, "expose-event", G_CALLBACK(on_expose_event), this);
#endif
    g_signal_connect(m_drawing_area, "button-press-event", G_CALLBACK(on_button_press_event), this);

    m_renderer.set_resolution(width, height);

    // Đăng ký timer 100ms định kỳ
    m_timer_id = g_timeout_add(100, on_timer_tick_event, this);

    gtk_widget_show_all(m_window);
}

void GtkTouchAdapter::run() {
    gtk_main();
}

#if GTK_CHECK_VERSION(3, 0, 0)
gboolean GtkTouchAdapter::on_draw_event([[maybe_unused]] GtkWidget* widget, cairo_t* cr, gpointer user_data) {
    auto* self = static_cast<GtkTouchAdapter*>(user_data);
    if (self) {
        self->m_renderer.draw_to_context(cr);
    }
    return FALSE;
}
#else
gboolean GtkTouchAdapter::on_expose_event(GtkWidget* widget, [[maybe_unused]] GdkEventExpose* event, gpointer user_data) {
    auto* self = static_cast<GtkTouchAdapter*>(user_data);
    if (self) {
#if GTK_CHECK_VERSION(2, 14, 0)
        GdkWindow* win = gtk_widget_get_window(widget);
#else
        GdkWindow* win = widget->window;
#endif
        cairo_t* cr = gdk_cairo_create(win);
        self->m_renderer.draw_to_context(cr);
        cairo_destroy(cr);
    }
    return FALSE;
}
#endif

gboolean GtkTouchAdapter::on_button_press_event([[maybe_unused]] GtkWidget* widget, GdkEventButton* event, gpointer user_data) {
    auto* self = static_cast<GtkTouchAdapter*>(user_data);
    if (!self || !event) return FALSE;

    // Chống bấm liên tục (Debounce 300ms)
    guint32 now = event->time;
    if (now - self->m_last_click_time < 300) {
        return TRUE;
    }
    self->m_last_click_time = now;

    self->m_interactor.on_touch(static_cast<int>(event->x), static_cast<int>(event->y));
    gtk_widget_queue_draw(self->m_drawing_area);
    return TRUE;
}

gboolean GtkTouchAdapter::on_timer_tick_event(gpointer user_data) {
    auto* self = static_cast<GtkTouchAdapter*>(user_data);
    if (self) {
        self->m_interactor.on_timer_tick(100);
        gtk_widget_queue_draw(self->m_drawing_area);
    }
    return G_SOURCE_CONTINUE;
}

} // namespace gach::adapters::ui