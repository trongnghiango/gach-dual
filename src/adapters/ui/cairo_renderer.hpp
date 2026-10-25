#pragma once

#include <cairo.h>
#include <string>
#include <string_view>
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

    // Phương thức vẽ trực tiếp lên Cairo Context được cung cấp từ GTK hoặc Offscreen Surface
    void draw_to_context(cairo_t* cr);

    [[nodiscard]] bool needs_full_refresh() const noexcept { return m_needs_full_refresh; }
    void clear_full_refresh_flag() noexcept { m_needs_full_refresh = false; }

private:
    void draw_image(cairo_t* cr, const std::string& image_path, const Rect& rect);
    void draw_text(cairo_t* cr, const std::string& text, const Rect& rect);
    void draw_phonics_word(cairo_t* cr, const domain::StoryBeat& beat, const domain::FlowFSM& fsm, const Rect& rect);
    void draw_choices(cairo_t* cr, const domain::StoryBeat& beat, const domain::FlowFSM& fsm);
    void draw_next_button(cairo_t* cr, const Rect& rect);

    UILayout& m_layout;
    const domain::StoryBeat* m_current_beat{nullptr};
    const domain::FlowFSM* m_current_fsm{nullptr};
    bool m_needs_full_refresh{true};
    int m_width{1072};
    int m_height{1448};
};

} // namespace gach::adapters::ui