#pragma once

#include <cairo.h>
#include <string>
#include <vector>
#include "application/ports/display_renderer_port.hpp"
#include "adapters/ui/ui_layout.hpp"

namespace gach::adapters::ui {

class CairoRenderer : public application::ports::IDisplayRenderer {
public:
    explicit CairoRenderer(UILayout& layout);
    ~CairoRenderer() override = default;

    // IDisplayRenderer Implementation
    void render_beat(const domain::StoryBeat& beat,
                     const domain::FlowFSM& fsm,
                     bool full_refresh) override;
    void request_redraw(bool full_refresh) override;
    void set_resolution(int width, int height) override;

    void draw_to_context(cairo_t* cr);

    [[nodiscard]] bool needs_full_refresh() const noexcept { return m_needs_full_refresh; }
    void clear_full_refresh_flag() noexcept { m_needs_full_refresh = false; }

    void set_overlay_state(OverlayMode mode, bool auto_hint, const std::vector<std::string>& words) override {
        m_overlay_mode = mode;
        m_auto_hint = auto_hint;
        m_story_words = words;
    }
    [[nodiscard]] OverlayMode overlay_mode() const noexcept { return m_overlay_mode; }

private:
    void draw_top_bar(cairo_t* cr);
    void draw_image(cairo_t* cr, const std::string& image_path, const Rect& rect);
    void draw_story_text(cairo_t* cr, const std::string& text, const Rect& rect);
    void draw_phonics_word(cairo_t* cr, const domain::StoryBeat& beat, const domain::FlowFSM& fsm, const Rect& rect);
    void draw_choices(cairo_t* cr, const domain::StoryBeat& beat, const domain::FlowFSM& fsm);
    void draw_next_button(cairo_t* cr, const Rect& rect);

    static void draw_rounded_rect(cairo_t* cr, double x, double y, double w, double h, double r);
    static std::vector<std::string> wrap_text_lines(cairo_t* cr, const std::string& text, double max_width);

    UILayout& m_layout;
    const domain::StoryBeat* m_current_beat{nullptr};
    const domain::FlowFSM* m_current_fsm{nullptr};
    bool m_needs_full_refresh{true};
    int m_width{1072};
    int m_height{1448};
    OverlayMode m_overlay_mode{OverlayMode::NONE};
    bool m_auto_hint{true};
    std::vector<std::string> m_story_words{};
};

} // namespace gach::adapters::ui
