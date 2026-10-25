#pragma once

#include <gtk/gtk.h>

#ifndef G_SOURCE_CONTINUE
#define G_SOURCE_CONTINUE TRUE
#endif
#ifndef G_SOURCE_REMOVE
#define G_SOURCE_REMOVE FALSE
#endif

#include "application/story_interactor.hpp"
#include "adapters/ui/cairo_renderer.hpp"

namespace gach::adapters::ui {

class GtkTouchAdapter {
public:
    GtkTouchAdapter(application::StoryInteractor& interactor, CairoRenderer& renderer);
    ~GtkTouchAdapter() = default;

    void initialize(int argc, char** argv, int width, int height);
    void run();

private:
#if GTK_CHECK_VERSION(3, 0, 0)
    static gboolean on_draw_event(GtkWidget* widget, cairo_t* cr, gpointer user_data);
#else
    static gboolean on_expose_event(GtkWidget* widget, GdkEventExpose* event, gpointer user_data);
#endif
    static gboolean on_button_press_event(GtkWidget* widget, GdkEventButton* event, gpointer user_data);
    static gboolean on_timer_tick_event(gpointer user_data);

    application::StoryInteractor& m_interactor;
    CairoRenderer& m_renderer;
    GtkWidget* m_window{nullptr};
    GtkWidget* m_drawing_area{nullptr};
    guint m_timer_id{0};
    guint32 m_last_click_time{0};

    int m_virtual_w{1072};
    int m_virtual_h{1448};
    double m_scale{1.0};
    double m_offset_x{0.0};
    double m_offset_y{0.0};
};

} // namespace gach::adapters::ui
