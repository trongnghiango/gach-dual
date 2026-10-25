#include "adapters/ui/cairo_renderer.hpp"
#include "adapters/ui/minimal_components.hpp"
#include <cmath>
#include <iostream>
#include <sstream>
#include <unistd.h>

namespace gach::adapters::ui {

CairoRenderer::CairoRenderer(UILayout& layout)
    : m_layout(layout) {}

void CairoRenderer::set_resolution(int width, int height) {
    m_width = width;
    m_height = height;
    m_layout.update_resolution(width, height);
}

void CairoRenderer::render_beat(const domain::StoryBeat& beat,
                                const domain::FlowFSM& fsm,
                                bool full_refresh) {
    m_current_beat = &beat;
    m_current_fsm = &fsm;
    if (full_refresh) {
        m_needs_full_refresh = true;
    }
}

void CairoRenderer::request_redraw(bool full_refresh) {
    if (full_refresh) {
        m_needs_full_refresh = true;
    }
}

void CairoRenderer::draw_rounded_rect(cairo_t* cr, double x, double y, double w, double h, double r) {
    MinimalComponents::draw_rounded_rect(cr, x, y, w, h, r);
}

std::vector<std::string> CairoRenderer::wrap_text_lines(cairo_t* cr, const std::string& text, double max_width) {
    std::vector<std::string> lines;
    std::istringstream stream(text);
    std::string word;
    std::string current_line;

    while (stream >> word) {
        std::string test_line = current_line.empty() ? word : (current_line + " " + word);
        cairo_text_extents_t ext;
        cairo_text_extents(cr, test_line.c_str(), &ext);

        if (ext.width > max_width && !current_line.empty()) {
            lines.push_back(current_line);
            current_line = word;
        } else {
            current_line = test_line;
        }
    }
    if (!current_line.empty()) {
        lines.push_back(current_line);
    }
    return lines;
}

void CairoRenderer::draw_to_context(cairo_t* cr) {
    if (!cr) return;

    // 1. Nền giấy trắng E-Ink thuần khiết (Pure minimalism paper background)
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_paint(cr);

    if (!m_current_beat || !m_current_fsm) {
        cairo_set_source_rgb(cr, 0.3, 0.3, 0.3);
        cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
        cairo_set_font_size(cr, 32.0);
        cairo_move_to(cr, 80, 300);
        cairo_show_text(cr, "Dang nap truyen... Be cho mot chut nhe!");
        return;
    }

    // 2. Chỉ báo trang và nút tiện ích ở đỉnh
    draw_top_bar(cr);

    // 3. Tranh minh họa Ehon tự nhiên hòa vào trang giấy
    const std::string& img_path = (m_current_fsm->state() == domain::BeatState::TRANSFORM ||
                                   m_current_fsm->state() == domain::BeatState::COMPLETED)
                                  ? m_current_beat->transform_image_path()
                                  : m_current_beat->initial_image_path();
    draw_image(cr, img_path, m_layout.image_rect());

    // 4. Lời dẫn truyện với typography rõ ràng, không chân, thoáng đãng
    draw_story_text(cr, m_current_beat->lead_text(), m_layout.text_rect());

    // 5. Thẻ tiêu điểm ghép vần tối giản (Phonics Focus Slot)
    draw_phonics_word(cr, *m_current_beat, *m_current_fsm, m_layout.phonics_word_rect());

    // 6. Các mảnh ghép lựa chọn tối giản (Chỉ hiện khi AWAIT_INPUT hoặc TRANSFORM)
    if (m_current_fsm->state() == domain::BeatState::AWAIT_INPUT ||
        m_current_fsm->state() == domain::BeatState::TRANSFORM) {
        draw_choices(cr, *m_current_beat, *m_current_fsm);
    }

    // 7. Nút tiếp tục sang trang khi ở nhịp COMPLETED
    if (m_current_fsm->state() == domain::BeatState::COMPLETED) {
        draw_next_button(cr, m_layout.next_button_rect());
    }

    // 8. Nếu đang mở Mục Lục hoặc Cài Đặt, vẽ Modal Overlay lên trên cùng
    if (m_overlay_mode == OverlayMode::MENU_TOC) {
        MinimalComponents::draw_toc_overlay(cr, m_width, m_height, m_current_beat ? m_current_beat->page_number() : 1, m_story_words);
    } else if (m_overlay_mode == OverlayMode::SETTINGS) {
        MinimalComponents::draw_settings_overlay(cr, m_width, m_height, m_auto_hint);
    }
}

void CairoRenderer::draw_top_bar(cairo_t* cr) {
    if (!m_current_beat) return;

    // 1. Nút Quay lại (←) ở bên trái
    Rect br = m_layout.back_button_rect();
    bool can_go_back = (m_current_beat->page_number() > 1);
    MinimalComponents::draw_utility_pill(cr, br.x, br.y, br.width, br.height,
                                        "Quay lai", MinimalComponents::UtilityIcon::BACK, !can_go_back);

    // 2. Nút Mục lục (☰)
    Rect mr = m_layout.menu_button_rect();
    MinimalComponents::draw_utility_pill(cr, mr.x, mr.y, mr.width, mr.height,
                                        "Muc luc", MinimalComponents::UtilityIcon::MENU);

    // 3. Chỉ số trang thanh nhã ở chính giữa đỉnh
    MinimalComponents::draw_page_tracker(cr, m_width / 2.0, br.y + br.height - 12.0,
                                         m_current_beat->page_number(),
                                         m_current_beat->total_pages());

    // 4. Nút Cài đặt (⚙)
    Rect sr = m_layout.settings_button_rect();
    MinimalComponents::draw_utility_pill(cr, sr.x, sr.y, sr.width, sr.height,
                                        "Cai dat", MinimalComponents::UtilityIcon::SETTINGS);

    // 5. Nút Thoát (✕) ở góc trên bên phải
    Rect er = m_layout.exit_button_rect();
    MinimalComponents::draw_utility_pill(cr, er.x, er.y, er.width, er.height,
                                        "Thoat", MinimalComponents::UtilityIcon::CLOSE);
}

void CairoRenderer::draw_image(cairo_t* cr, const std::string& image_path, const Rect& rect) {
    cairo_save(cr);

    bool file_exists = (!image_path.empty() && access(image_path.c_str(), F_OK) == 0);
    if (file_exists) {
        cairo_surface_t* img_surface = cairo_image_surface_create_from_png(image_path.c_str());
        if (cairo_surface_status(img_surface) == CAIRO_STATUS_SUCCESS) {
            int img_w = cairo_image_surface_get_width(img_surface);
            int img_h = cairo_image_surface_get_height(img_surface);

            double scale_x = static_cast<double>(rect.width) / img_w;
            double scale_y = static_cast<double>(rect.height) / img_h;
            double scale = std::min(scale_x, scale_y);

            double draw_w = img_w * scale;
            double draw_h = img_h * scale;
            double offset_x = rect.x + (rect.width - draw_w) / 2.0;
            double offset_y = rect.y + (rect.height - draw_h) / 2.0;

            // Nạp và vẽ ảnh sạch sẽ, frameless (hòa nhập hoàn toàn vào nền giấy)
            cairo_save(cr);
            cairo_translate(cr, offset_x, offset_y);
            cairo_scale(cr, scale, scale);
            cairo_set_source_surface(cr, img_surface, 0, 0);
            cairo_paint(cr);
            cairo_restore(cr);

            cairo_surface_destroy(img_surface);
            cairo_restore(cr);
            return;
        }
        cairo_surface_destroy(img_surface);
    }

    // Fallback nhẹ nhàng nếu thiếu tệp ảnh
    cairo_set_source_rgb(cr, 0.96, 0.96, 0.96);
    draw_rounded_rect(cr, rect.x, rect.y, rect.width, rect.height, 16.0);
    cairo_fill(cr);

    cairo_set_source_rgb(cr, 0.4, 0.4, 0.4);
    cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_ITALIC, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 26.0);
    cairo_move_to(cr, rect.x + 40, rect.y + rect.height / 2);
    cairo_show_text(cr, "[Buc tranh minh hoa ban Mit]");
    cairo_restore(cr);
}

void CairoRenderer::draw_story_text(cairo_t* cr, const std::string& text, const Rect& rect) {
    cairo_save(cr);
    cairo_set_source_rgb(cr, 0.05, 0.05, 0.05);
    cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);

    // Chữ to rõ ràng cho ba mẹ và bé cùng đọc thoải mái từ xa (Font 46px, line-height 66px)
    double font_size = 46.0;
    double line_height = 66.0;
    cairo_set_font_size(cr, font_size);

    auto lines = wrap_text_lines(cr, text, rect.width);

    double total_text_h = lines.size() * line_height;
    double start_y = rect.y + (rect.height - total_text_h) / 2.0 + font_size - 4;

    for (size_t i = 0; i < lines.size(); ++i) {
        cairo_text_extents_t ext;
        cairo_text_extents(cr, lines[i].c_str(), &ext);
        double line_x = rect.x + (rect.width - ext.width) / 2.0;
        double line_y = start_y + i * line_height;

        cairo_move_to(cr, line_x, line_y);
        cairo_show_text(cr, lines[i].c_str());
    }
    cairo_restore(cr);
}

void CairoRenderer::draw_phonics_word(cairo_t* cr, const domain::StoryBeat& beat, const domain::FlowFSM& fsm, const Rect& rect) {
    bool completed = (fsm.state() == domain::BeatState::TRANSFORM || fsm.state() == domain::BeatState::COMPLETED);
    bool missing_onset = (beat.missing_part() == domain::MissingPart::ONSET);

    MinimalComponents::draw_phonics_slot(cr, rect.x, rect.y, rect.width, rect.height,
                                        beat.syllable().onset(), beat.syllable().rhyme(),
                                        missing_onset, completed, beat.syllable().full_word());
}

void CairoRenderer::draw_choices(cairo_t* cr, const domain::StoryBeat& beat, const domain::FlowFSM& fsm) {
    uint8_t count = std::min(beat.choice_count(), static_cast<uint8_t>(3));
    for (uint8_t i = 0; i < count; ++i) {
        Rect r = m_layout.choice_rect(i);
        if (r.width <= 0 || r.height <= 0) continue;

        bool is_correct = beat.choices()[i].is_correct;
        bool is_hinted = fsm.hint_visible() && is_correct;

        auto state = is_hinted ? MinimalComponents::ChoiceState::HINTED : MinimalComponents::ChoiceState::NORMAL;
        MinimalComponents::draw_choice_tile(cr, r.x, r.y, r.width, r.height,
                                            beat.choices()[i].display_text(), state);
    }
}

void CairoRenderer::draw_next_button(cairo_t* cr, const Rect& rect) {
    MinimalComponents::draw_action_pill(cr, rect.x, rect.y, rect.width, rect.height,
                                        "Tiep tuc sang trang  ->",
                                        MinimalComponents::ButtonStyle::PRIMARY_SOLID);
}

} // namespace gach::adapters::ui
