#include "adapters/ui/cairo_renderer.hpp"
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
    cairo_new_sub_path(cr);
    cairo_arc(cr, x + w - r, y + r, r, -M_PI/2, 0);
    cairo_arc(cr, x + w - r, y + h - r, r, 0, M_PI/2);
    cairo_arc(cr, x + r, y + h - r, r, M_PI/2, M_PI);
    cairo_arc(cr, x + r, y + r, r, M_PI, 3*M_PI/2);
    cairo_close_path(cr);
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

    // 1. Nền trắng E-Ink thuần khiết (Pure minimalism paper background)
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

    // 2. Chỉ báo trang và nút thoát tinh gọn ở đỉnh
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
}

void CairoRenderer::draw_top_bar(cairo_t* cr) {
    if (!m_current_beat) return;
    cairo_save(cr);

    // Chỉ số trang thanh nhã ở chính giữa đỉnh
    cairo_set_source_rgb(cr, 0.45, 0.45, 0.45);
    cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 22.0);

    std::string page_info = std::to_string(m_current_beat->page_number()) +
                            "  /  " + std::to_string(m_current_beat->total_pages());
    cairo_text_extents_t ext;
    cairo_text_extents(cr, page_info.c_str(), &ext);

    double x = (m_width - ext.width) / 2.0;
    cairo_move_to(cr, x, 44);
    cairo_show_text(cr, page_info.c_str());

    // Nút Thoát tối giản ở góc trên bên phải
    Rect er = m_layout.exit_button_rect();

    // Viền pill mỏng nhẹ tinh tế
    cairo_set_source_rgb(cr, 0.85, 0.85, 0.85);
    cairo_set_line_width(cr, 1.5);
    draw_rounded_rect(cr, er.x, er.y, er.width, er.height, er.height / 2.0);
    cairo_stroke(cr);

    // Chữ 'Thoát'
    cairo_set_source_rgb(cr, 0.35, 0.35, 0.35);
    cairo_set_font_size(cr, 19.0);
    cairo_move_to(cr, er.x + 14, er.y + er.height - 12);
    cairo_show_text(cr, "Thoat");

    // Dấu X vector sắc nét
    double x_cx = er.x + er.width - 20;
    double x_cy = er.y + er.height / 2.0;
    double xs = 6.0;
    cairo_set_line_width(cr, 2.0);
    cairo_move_to(cr, x_cx - xs, x_cy - xs); cairo_line_to(cr, x_cx + xs, x_cy + xs);
    cairo_move_to(cr, x_cx + xs, x_cy - xs); cairo_line_to(cr, x_cx - xs, x_cy + xs);
    cairo_stroke(cr);

    cairo_restore(cr);
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

    // Chữ to rõ ràng, nét đều sans-serif, khoảng cách dòng thoáng đãng
    double font_size = 38.0;
    double line_height = 56.0;
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
    cairo_save(cr);

    // 1. Thẻ tiêu điểm ghép vần tối giản (Phonics Focus Container)
    cairo_set_source_rgb(cr, 0.98, 0.98, 0.98);
    draw_rounded_rect(cr, rect.x, rect.y, rect.width, rect.height, 16.0);
    cairo_fill(cr);

    cairo_set_source_rgb(cr, 0.82, 0.82, 0.82);
    cairo_set_line_width(cr, 1.5);
    draw_rounded_rect(cr, rect.x, rect.y, rect.width, rect.height, 16.0);
    cairo_stroke(cr);

    double center_y = rect.y + rect.height / 2.0;
    cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);

    if (fsm.state() == domain::BeatState::TRANSFORM || fsm.state() == domain::BeatState::COMPLETED) {
        // Trạng thái đã ghép đúng: Từ hoàn chỉnh hiển thị cực kỳ sang trọng và nổi bật
        cairo_set_font_size(cr, 72.0);
        cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);

        std::string full_word = std::string(beat.syllable().full_word());
        cairo_text_extents_t ext;
        cairo_text_extents(cr, full_word.c_str(), &ext);

        double tx = rect.x + (rect.width - ext.width) / 2.0;
        double ty = center_y + ext.height / 2.0 - 4;
        cairo_move_to(cr, tx, ty);
        cairo_show_text(cr, full_word.c_str());
    } else {
        // Trạng thái chờ ghép: Rãnh ghép vần trực giác không rườm rà
        double slot_w = 110.0;
        double slot_h = 68.0;
        double slot_r = 12.0;

        cairo_set_font_size(cr, 66.0);
        cairo_text_extents_t letter_ext;

        if (beat.missing_part() == domain::MissingPart::ONSET) {
            // Khuyết âm đầu: [ ? ] ở bên trái, vần ở bên phải
            std::string rhyme_str = std::string(beat.syllable().rhyme());
            cairo_text_extents(cr, rhyme_str.c_str(), &letter_ext);

            double spacing = 24.0;
            double total_w = slot_w + spacing + letter_ext.width;
            double start_x = rect.x + (rect.width - total_w) / 2.0;

            double slot_x = start_x;
            double slot_y = center_y - slot_h / 2.0;

            // Nền trắng bên trong slot
            cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
            draw_rounded_rect(cr, slot_x, slot_y, slot_w, slot_h, slot_r);
            cairo_fill(cr);

            // Viền nét đứt tinh tế
            const double dashes[] = {6.0, 4.0};
            cairo_set_dash(cr, dashes, 2, 0);
            cairo_set_source_rgb(cr, 0.4, 0.4, 0.4);
            cairo_set_line_width(cr, 2.0);
            draw_rounded_rect(cr, slot_x, slot_y, slot_w, slot_h, slot_r);
            cairo_stroke(cr);
            cairo_set_dash(cr, nullptr, 0, 0);

            // Dấu hỏi '?' xám nhạt
            cairo_set_source_rgb(cr, 0.55, 0.55, 0.55);
            cairo_set_font_size(cr, 42.0);
            cairo_text_extents_t q_ext;
            cairo_text_extents(cr, "?", &q_ext);
            cairo_move_to(cr, slot_x + (slot_w - q_ext.width) / 2.0, slot_y + (slot_h + q_ext.height) / 2.0 - 2);
            cairo_show_text(cr, "?");

            // Vần ở bên phải
            cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
            cairo_set_font_size(cr, 66.0);
            double lx = start_x + slot_w + spacing;
            double ly = center_y + letter_ext.height / 2.0 - 4;
            cairo_move_to(cr, lx, ly);
            cairo_show_text(cr, rhyme_str.c_str());

        } else {
            // Khuyết vần: Âm đầu ở bên trái, [ ? ] ở bên phải
            std::string onset_str = std::string(beat.syllable().onset());
            cairo_text_extents(cr, onset_str.c_str(), &letter_ext);

            double spacing = 24.0;
            double total_w = letter_ext.width + spacing + slot_w;
            double start_x = rect.x + (rect.width - total_w) / 2.0;

            // Âm đầu ở bên trái
            cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
            cairo_set_font_size(cr, 66.0);
            double lx = start_x;
            double ly = center_y + letter_ext.height / 2.0 - 4;
            cairo_move_to(cr, lx, ly);
            cairo_show_text(cr, onset_str.c_str());

            // Slot ở bên phải
            double slot_x = start_x + letter_ext.width + spacing;
            double slot_y = center_y - slot_h / 2.0;

            cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
            draw_rounded_rect(cr, slot_x, slot_y, slot_w, slot_h, slot_r);
            cairo_fill(cr);

            const double dashes[] = {6.0, 4.0};
            cairo_set_dash(cr, dashes, 2, 0);
            cairo_set_source_rgb(cr, 0.4, 0.4, 0.4);
            cairo_set_line_width(cr, 2.0);
            draw_rounded_rect(cr, slot_x, slot_y, slot_w, slot_h, slot_r);
            cairo_stroke(cr);
            cairo_set_dash(cr, nullptr, 0, 0);

            cairo_set_source_rgb(cr, 0.55, 0.55, 0.55);
            cairo_set_font_size(cr, 42.0);
            cairo_text_extents_t q_ext;
            cairo_text_extents(cr, "?", &q_ext);
            cairo_move_to(cr, slot_x + (slot_w - q_ext.width) / 2.0, slot_y + (slot_h + q_ext.height) / 2.0 - 2);
            cairo_show_text(cr, "?");
        }
    }

    cairo_restore(cr);
}

void CairoRenderer::draw_choices(cairo_t* cr, const domain::StoryBeat& beat, const domain::FlowFSM& fsm) {
    uint8_t count = std::min(beat.choice_count(), static_cast<uint8_t>(3));
    for (uint8_t i = 0; i < count; ++i) {
        Rect r = m_layout.choice_rect(i);
        if (r.width <= 0 || r.height <= 0) continue;
        bool is_correct = beat.choices()[i].is_correct;
        bool is_hinted = fsm.hint_visible() && is_correct;

        cairo_save(cr);

        // Mảnh ghép chữ cái tối giản: Nền trắng tinh khiết, viền mảnh 2px, không bóng đổ
        if (is_hinted) {
            // High-contrast Inversion khi có gợi ý dẫn lối: Nền đen chữ trắng
            cairo_set_source_rgb(cr, 0.05, 0.05, 0.05);
            draw_rounded_rect(cr, r.x, r.y, r.width, r.height, 18.0);
            cairo_fill(cr);
        } else {
            cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
            draw_rounded_rect(cr, r.x, r.y, r.width, r.height, 18.0);
            cairo_fill(cr);

            cairo_set_source_rgb(cr, 0.15, 0.15, 0.15);
            cairo_set_line_width(cr, 2.0);
            draw_rounded_rect(cr, r.x, r.y, r.width, r.height, 18.0);
            cairo_stroke(cr);
        }

        // Chữ cái to tròn dứt khoát ở chính giữa
        if (is_hinted) {
            cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
        } else {
            cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
        }

        cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
        cairo_set_font_size(cr, 82.0);

        std::string_view text = beat.choices()[i].display_text();
        cairo_text_extents_t ext;
        cairo_text_extents(cr, std::string(text).c_str(), &ext);

        double tx = r.x + (r.width - ext.width) / 2.0 - ext.x_bearing;
        double ty = r.y + (r.height - ext.height) / 2.0 - ext.y_bearing;

        cairo_move_to(cr, tx, ty);
        cairo_show_text(cr, std::string(text).c_str());
        cairo_restore(cr);
    }
}

void CairoRenderer::draw_next_button(cairo_t* cr, const Rect& rect) {
    cairo_save(cr);

    // Nút chuyển trang tối giản: Solid black pill button (High-contrast primary CTA)
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    draw_rounded_rect(cr, rect.x, rect.y, rect.width, rect.height, rect.height / 2.0);
    cairo_fill(cr);

    // Chữ trắng sắc nét, dứt khoát
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 36.0);

    std::string text = "Tiếp tục sang trang  ->";
    cairo_text_extents_t ext;
    cairo_text_extents(cr, text.c_str(), &ext);

    double tx = rect.x + (rect.width - ext.width) / 2.0;
    double ty = rect.y + (rect.height + ext.height) / 2.0 - 4;

    cairo_move_to(cr, tx, ty);
    cairo_show_text(cr, text.c_str());

    cairo_restore(cr);
}

} // namespace gach::adapters::ui
