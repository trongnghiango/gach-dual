#include "adapters/ui/cairo_renderer.hpp"
#include <cmath>
#include <iostream>
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

void CairoRenderer::draw_to_context(cairo_t* cr) {
    if (!cr) return;

    // 1. Tô nền giấy trắng E-Ink (Trắng tuyệt đối: 1.0, 1.0, 1.0)
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_paint(cr);

    if (!m_current_beat || !m_current_fsm) {
        // Trạng thái trống (Empty State)
        cairo_set_source_rgb(cr, 0.2, 0.2, 0.2);
        cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
        cairo_set_font_size(cr, 28.0);
        cairo_move_to(cr, 80, 200);
        cairo_show_text(cr, "Mít đang chuẩn bị sách... Bé chờ một chút nhé!");
        return;
    }

    // 2. Vẽ tranh minh họa
    // Nếu ở nhịp TRANSFORM hoặc COMPLETED: vẽ tranh quả ngọt; ngược lại vẽ tranh khởi đầu
    const std::string& img_path = (m_current_fsm->state() == domain::BeatState::TRANSFORM ||
                                   m_current_fsm->state() == domain::BeatState::COMPLETED)
                                  ? m_current_beat->transform_image_path()
                                  : m_current_beat->initial_image_path();
    draw_image(cr, img_path, m_layout.image_rect());

    // 3. Vẽ lời dẫn truyện
    draw_text(cr, m_current_beat->lead_text(), m_layout.text_rect());

    // 4. Vẽ từ vựng mục tiêu với vần khuyết (Visual Phonics)
    draw_phonics_word(cr, *m_current_beat, *m_current_fsm, m_layout.phonics_word_rect());

    // 5. Vẽ các mảnh ghép lựa chọn (Chỉ vẽ khi ở nhịp AWAIT_INPUT hoặc TRANSFORM)
    if (m_current_fsm->state() == domain::BeatState::AWAIT_INPUT ||
        m_current_fsm->state() == domain::BeatState::TRANSFORM) {
        draw_choices(cr, *m_current_beat, *m_current_fsm);
    }

    // 6. Vẽ nút Next Page khi ở nhịp COMPLETED
    if (m_current_fsm->state() == domain::BeatState::COMPLETED) {
        draw_next_button(cr, m_layout.next_button_rect());
    }
}

void CairoRenderer::draw_image(cairo_t* cr, const std::string& image_path, const Rect& rect) {
    // Kiểm tra tệp tồn tại theo chuẩn FAT32 access(F_OK)
    bool file_exists = (!image_path.empty() && access(image_path.c_str(), F_OK) == 0);

    if (file_exists) {
        cairo_surface_t* img_surface = cairo_image_surface_create_from_png(image_path.c_str());
        if (cairo_surface_status(img_surface) == CAIRO_STATUS_SUCCESS) {
            int img_w = cairo_image_surface_get_width(img_surface);
            int img_h = cairo_image_surface_get_height(img_surface);

            cairo_save(cr);
            double scale_x = static_cast<double>(rect.width) / img_w;
            double scale_y = static_cast<double>(rect.height) / img_h;
            double scale = std::min(scale_x, scale_y);

            double offset_x = rect.x + (rect.width - img_w * scale) / 2.0;
            double offset_y = rect.y + (rect.height - img_h * scale) / 2.0;

            cairo_translate(cr, offset_x, offset_y);
            cairo_scale(cr, scale, scale);
            cairo_set_source_surface(cr, img_surface, 0, 0);
            cairo_paint(cr);
            cairo_restore(cr);

            cairo_surface_destroy(img_surface);
            return;
        }
        cairo_surface_destroy(img_surface);
    }

    // Fallback khi tệp ảnh chưa có hoặc đang trong môi trường dev: Vẽ khung nét mực phác thảo
    cairo_set_source_rgb(cr, 0.92, 0.92, 0.92);
    cairo_rectangle(cr, rect.x, rect.y, rect.width, rect.height);
    cairo_fill(cr);

    cairo_set_source_rgb(cr, 0.4, 0.4, 0.4);
    cairo_set_line_width(cr, 2.0);
    cairo_rectangle(cr, rect.x, rect.y, rect.width, rect.height);
    cairo_stroke(cr);

    cairo_set_source_rgb(cr, 0.3, 0.3, 0.3);
    cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_ITALIC, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 22.0);
    cairo_move_to(cr, rect.x + 30, rect.y + rect.height / 2);
    cairo_show_text(cr, "[Bức tranh minh họa bạn Mít và khu vườn]");
}

void CairoRenderer::draw_text(cairo_t* cr, const std::string& text, const Rect& rect) {
    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
    cairo_select_font_face(cr, "serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 28.0);

    // Đơn giản hóa ngắt dòng cơ bản cho lời dẫn
    cairo_move_to(cr, rect.x, rect.y + 35);
    cairo_show_text(cr, text.c_str());
}

void CairoRenderer::draw_phonics_word(cairo_t* cr, const domain::StoryBeat& beat, const domain::FlowFSM& fsm, const Rect& rect) {
    cairo_save(cr);

    // Nền nhẹ cho vùng từ vựng
    cairo_set_source_rgb(cr, 0.96, 0.96, 0.96);
    cairo_rectangle(cr, rect.x, rect.y, rect.width, rect.height);
    cairo_fill(cr);

    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
    cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 36.0);

    std::string word_display;
    if (fsm.state() == domain::BeatState::TRANSFORM || fsm.state() == domain::BeatState::COMPLETED) {
        // Khi đã ghép đúng hoặc hoàn thành: Hiện từ vựng đầy đủ và trọn vẹn
        word_display = "Từ đúng: " + std::string(beat.syllable().full_word());
    } else {
        // Khi đang ở nhịp chờ bé ghép: Hiện vần khuyết với dấu ô vuông đứt nét
        if (beat.missing_part() == domain::MissingPart::ONSET) {
            word_display = "Ghép chữ: [ ... ] + " + std::string(beat.syllable().rhyme());
        } else {
            word_display = "Ghép chữ: " + std::string(beat.syllable().onset()) + " + [ ... ]";
        }
    }

    cairo_move_to(cr, rect.x + 20, rect.y + rect.height / 2 + 12);
    cairo_show_text(cr, word_display.c_str());
    cairo_restore(cr);
}

void CairoRenderer::draw_choices(cairo_t* cr, const domain::StoryBeat& beat, const domain::FlowFSM& fsm) {
    for (uint8_t i = 0; i < beat.choice_count(); ++i) {
        Rect r = m_layout.choice_rect(i);
        bool is_correct_choice = beat.choices()[i].is_correct;

        cairo_save(cr);
        // Nếu đang ở nhịp hint và đây là lựa chọn đúng -> Tô đậm viền dẫn lối
        if (fsm.hint_visible() && is_correct_choice) {
            cairo_set_source_rgb(cr, 0.88, 0.88, 0.88);
            cairo_rectangle(cr, r.x, r.y, r.width, r.height);
            cairo_fill(cr);
            cairo_set_line_width(cr, 6.0);
        } else {
            cairo_set_line_width(cr, 3.0);
        }

        cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
        cairo_rectangle(cr, r.x, r.y, r.width, r.height);
        cairo_stroke(cr);

        // Chữ cái trên mảnh ghép: Font tròn to, dễ nhìn cho trẻ
        cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
        cairo_set_font_size(cr, 42.0);

        std::string_view choice_text = beat.choices()[i].display_text();
        cairo_text_extents_t extents;
        cairo_text_extents(cr, std::string(choice_text).c_str(), &extents);

        double tx = r.x + (r.width - extents.width) / 2.0 - extents.x_bearing;
        double ty = r.y + (r.height - extents.height) / 2.0 - extents.y_bearing;

        cairo_move_to(cr, tx, ty);
        cairo_show_text(cr, std::string(choice_text).c_str());
        cairo_restore(cr);
    }
}

void CairoRenderer::draw_next_button(cairo_t* cr, const Rect& rect) {
    cairo_save(cr);
    // Nút tròn sang trang với mũi tên nét mực
    double center_x = rect.x + rect.width / 2.0;
    double center_y = rect.y + rect.height / 2.0;
    double radius = rect.width / 2.0 - 4.0;

    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    cairo_set_line_width(cr, 4.0);
    cairo_arc(cr, center_x, center_y, radius, 0, 2 * M_PI);
    cairo_stroke(cr);

    // Vẽ mũi tên ->
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    cairo_set_line_width(cr, 5.0);
    cairo_move_to(cr, center_x - 18, center_y);
    cairo_line_to(cr, center_x + 18, center_y);
    cairo_line_to(cr, center_x + 6, center_y - 12);
    cairo_move_to(cr, center_x + 18, center_y);
    cairo_line_to(cr, center_x + 6, center_y + 12);
    cairo_stroke(cr);

    cairo_restore(cr);
}

} // namespace gach::adapters::ui