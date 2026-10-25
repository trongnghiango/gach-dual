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

    // 1. Phủ toàn bộ màn hình bằng màu trắng E-Ink tinh khiết
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_paint(cr);

    // 1.1. Khung căn chỉnh sát mép viền vật lý màn hình (Physical Screen Boundary)
    // Giúp xác định chính xác mép màn hình và phát hiện thanh trạng thái Kindle OS đẩy xuống
    cairo_save(cr);
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    cairo_set_line_width(cr, 3.0);
    cairo_rectangle(cr, 2.0, 2.0, m_width - 4.0, m_height - 4.0);
    cairo_stroke(cr);

    // Vạch căn góc (Corner Alignment Marks) tại 4 góc mép
    double mark_len = 35.0;
    cairo_set_line_width(cr, 4.0);
    // Góc trên - trái (0, 0)
    cairo_move_to(cr, 2, 16); cairo_line_to(cr, mark_len, 16);
    cairo_move_to(cr, 16, 2); cairo_line_to(cr, 16, mark_len);
    // Góc trên - phải (W, 0)
    cairo_move_to(cr, m_width - mark_len, 16); cairo_line_to(cr, m_width - 2, 16);
    cairo_move_to(cr, m_width - 16, 2); cairo_line_to(cr, m_width - 16, mark_len);
    // Góc dưới - trái (0, H)
    cairo_move_to(cr, 2, m_height - 16); cairo_line_to(cr, mark_len, m_height - 16);
    cairo_move_to(cr, 16, m_height - mark_len); cairo_line_to(cr, 16, m_height - 2);
    // Góc dưới - phải (W, H)
    cairo_move_to(cr, m_width - mark_len, m_height - 16); cairo_line_to(cr, m_width - 2, m_height - 16);
    cairo_move_to(cr, m_width - 16, m_height - mark_len); cairo_line_to(cr, m_width - 16, m_height - 2);
    cairo_stroke(cr);
    cairo_restore(cr);

    if (!m_current_beat || !m_current_fsm) {
        cairo_set_source_rgb(cr, 0.2, 0.2, 0.2);
        cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
        cairo_set_font_size(cr, 36.0);
        cairo_move_to(cr, 80, 300);
        cairo_show_text(cr, "Mít đang chuẩn bị sách... Bé chờ một chút nhé!");
        return;
    }

    // 2. Vẽ thanh chỉ báo tiến trình trang nhẹ nhàng ở đỉnh
    draw_top_bar(cr);

    // 3. Vẽ tranh minh họa thuần khiết (không bị chữ đè)
    const std::string& img_path = (m_current_fsm->state() == domain::BeatState::TRANSFORM ||
                                   m_current_fsm->state() == domain::BeatState::COMPLETED)
                                  ? m_current_beat->transform_image_path()
                                  : m_current_beat->initial_image_path();
    draw_image(cr, img_path, m_layout.image_rect());

    // 4. Vẽ lời dẫn truyện với tự động ngắt dòng chữ to, căn giữa trang sách
    draw_story_text(cr, m_current_beat->lead_text(), m_layout.text_rect());

    // 5. Vẽ thẻ âm vị ghép vần to rõ (Visual Phonics)
    draw_phonics_word(cr, *m_current_beat, *m_current_fsm, m_layout.phonics_word_rect());

    // 6. Vẽ các mảnh ghép lựa chọn (Chỉ hiện khi ở nhịp AWAIT_INPUT hoặc TRANSFORM)
    if (m_current_fsm->state() == domain::BeatState::AWAIT_INPUT ||
        m_current_fsm->state() == domain::BeatState::TRANSFORM) {
        draw_choices(cr, *m_current_beat, *m_current_fsm);
    }

    // 7. Vẽ nút chuyển trang rộng rãi khi ở nhịp COMPLETED
    if (m_current_fsm->state() == domain::BeatState::COMPLETED) {
        draw_next_button(cr, m_layout.next_button_rect());
    }
}

void CairoRenderer::draw_top_bar(cairo_t* cr) {
    if (!m_current_beat) return;
    cairo_save(cr);
    cairo_set_source_rgb(cr, 0.45, 0.45, 0.45);
    cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 24.0);

    std::string page_info = "—  Trang " + std::to_string(m_current_beat->page_number()) + " / 3  —";
    cairo_text_extents_t ext;
    cairo_text_extents(cr, page_info.c_str(), &ext);

    double x = (m_width - ext.width) / 2.0;
    cairo_move_to(cr, x, 32);
    cairo_show_text(cr, page_info.c_str());
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

            double scale_x = static_cast<double>(rect.width - 8) / img_w;
            double scale_y = static_cast<double>(rect.height - 8) / img_h;
            double scale = std::min(scale_x, scale_y);

            double draw_w = img_w * scale;
            double draw_h = img_h * scale;
            double offset_x = rect.x + (rect.width - draw_w) / 2.0;
            double offset_y = rect.y + (rect.height - draw_h) / 2.0;

            // Nạp và vẽ ảnh
            cairo_save(cr);
            cairo_translate(cr, offset_x, offset_y);
            cairo_scale(cr, scale, scale);
            cairo_set_source_surface(cr, img_surface, 0, 0);
            cairo_paint(cr);
            cairo_restore(cr);

            // Viền bo góc ôm trọn ảnh minh họa
            cairo_set_source_rgb(cr, 0.15, 0.15, 0.15);
            cairo_set_line_width(cr, 3.5);
            draw_rounded_rect(cr, offset_x - 3, offset_y - 3, draw_w + 6, draw_h + 6, 16.0);
            cairo_stroke(cr);

            cairo_surface_destroy(img_surface);
            cairo_restore(cr);
            return;
        }
        cairo_surface_destroy(img_surface);
    }

    // Fallback nếu thiếu tệp ảnh
    cairo_set_source_rgb(cr, 0.95, 0.95, 0.95);
    draw_rounded_rect(cr, rect.x, rect.y, rect.width, rect.height, 16.0);
    cairo_fill(cr);

    cairo_set_source_rgb(cr, 0.3, 0.3, 0.3);
    cairo_set_line_width(cr, 3.0);
    draw_rounded_rect(cr, rect.x, rect.y, rect.width, rect.height, 16.0);
    cairo_stroke(cr);

    cairo_set_source_rgb(cr, 0.3, 0.3, 0.3);
    cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_ITALIC, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 28.0);
    cairo_move_to(cr, rect.x + 40, rect.y + rect.height / 2);
    cairo_show_text(cr, "[Bức tranh minh họa bạn Mít]");
    cairo_restore(cr);
}

void CairoRenderer::draw_story_text(cairo_t* cr, const std::string& text, const Rect& rect) {
    cairo_save(cr);
    // Màu đen tuyệt đối (High-contrast E-Ink)
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    // Font không chân (Sans-Serif) tròn trịa, dứt khoát, dễ đọc cho trẻ, không râu ria
    cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);

    // Chữ to rõ ràng, nét dứt khoát trên màn hình 300 PPI
    double font_size = 44.0;
    double line_height = 62.0;
    cairo_set_font_size(cr, font_size);

    auto lines = wrap_text_lines(cr, text, rect.width);

    // Tính toán căn giữa theo chiều dọc trong vùng văn bản
    double total_text_h = lines.size() * line_height;
    double start_y = rect.y + (rect.height - total_text_h) / 2.0 + font_size;

    for (size_t i = 0; i < lines.size(); ++i) {
        cairo_text_extents_t ext;
        cairo_text_extents(cr, lines[i].c_str(), &ext);
        // Căn giữa từng dòng
        double line_x = rect.x + (rect.width - ext.width) / 2.0;
        double line_y = start_y + i * line_height;

        cairo_move_to(cr, line_x, line_y);
        cairo_show_text(cr, lines[i].c_str());
    }
    cairo_restore(cr);
}

void CairoRenderer::draw_phonics_word(cairo_t* cr, const domain::StoryBeat& beat, const domain::FlowFSM& fsm, const Rect& rect) {
    cairo_save(cr);

    // 1. Vẽ thẻ nền bo góc (Card container)
    cairo_set_source_rgb(cr, 0.95, 0.95, 0.95);
    draw_rounded_rect(cr, rect.x, rect.y, rect.width, rect.height, 16.0);
    cairo_fill(cr);

    cairo_set_source_rgb(cr, 0.2, 0.2, 0.2);
    cairo_set_line_width(cr, 2.5);
    draw_rounded_rect(cr, rect.x, rect.y, rect.width, rect.height, 16.0);
    cairo_stroke(cr);

    // 2. Nội dung thẻ
    cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);

    if (fsm.state() == domain::BeatState::TRANSFORM || fsm.state() == domain::BeatState::COMPLETED) {
        // Trạng thái đã ghép đúng / hoàn thành: Chữ to nổi bật, đậm đà
        cairo_set_font_size(cr, 56.0);
        cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);

        std::string text = "Từ hoàn chỉnh:   " + std::string(beat.syllable().full_word());
        cairo_text_extents_t ext;
        cairo_text_extents(cr, text.c_str(), &ext);

        double tx = rect.x + (rect.width - ext.width) / 2.0;
        double ty = rect.y + (rect.height + ext.height) / 2.0 - 4;
        cairo_move_to(cr, tx, ty);
        cairo_show_text(cr, text.c_str());
    } else {
        // Trạng thái chờ ghép: Hiển thị công thức trực quan to rõ
        cairo_set_font_size(cr, 50.0);
        cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);

        std::string label;
        if (beat.missing_part() == domain::MissingPart::ONSET) {
            label = "Ghép chữ:   [ ? ]  +  " + std::string(beat.syllable().rhyme());
        } else {
            label = "Ghép chữ:   " + std::string(beat.syllable().onset()) + "  +  [ ? ]";
        }

        cairo_text_extents_t ext;
        cairo_text_extents(cr, label.c_str(), &ext);

        double tx = rect.x + (rect.width - ext.width) / 2.0;
        double ty = rect.y + (rect.height + ext.height) / 2.0 - 4;
        cairo_move_to(cr, tx, ty);
        cairo_show_text(cr, label.c_str());
    }

    cairo_restore(cr);
}

void CairoRenderer::draw_choices(cairo_t* cr, const domain::StoryBeat& beat, const domain::FlowFSM& fsm) {
    for (uint8_t i = 0; i < beat.choice_count(); ++i) {
        Rect r = m_layout.choice_rect(i);
        bool is_correct = beat.choices()[i].is_correct;

        cairo_save(cr);

        // Nền thẻ lựa chọn
        if (fsm.hint_visible() && is_correct) {
            // Hướng dẫn dẫn lối: Nền xám đậm hơn một chút
            cairo_set_source_rgb(cr, 0.88, 0.88, 0.88);
            draw_rounded_rect(cr, r.x, r.y, r.width, r.height, 22.0);
            cairo_fill(cr);

            // Viền đậm nổi bật
            cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
            cairo_set_line_width(cr, 6.0);
            draw_rounded_rect(cr, r.x, r.y, r.width, r.height, 22.0);
            cairo_stroke(cr);
        } else {
            cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
            draw_rounded_rect(cr, r.x, r.y, r.width, r.height, 22.0);
            cairo_fill(cr);

            cairo_set_source_rgb(cr, 0.15, 0.15, 0.15);
            cairo_set_line_width(cr, 4.0);
            draw_rounded_rect(cr, r.x, r.y, r.width, r.height, 22.0);
            cairo_stroke(cr);
        }

        // Chữ cái to rõ nét (Font 78px bold sans-serif) cho trẻ dễ nhìn, không râu ria
        cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
        cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
        cairo_set_font_size(cr, 78.0);

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

    // Nút dạng thanh Pill bo tròn nằm ở đáy
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    cairo_set_line_width(cr, 4.0);
    draw_rounded_rect(cr, rect.x, rect.y, rect.width, rect.height, rect.height / 2.0);
    cairo_stroke(cr);

    // Nền trắng bên trong nút
    cairo_set_source_rgb(cr, 0.95, 0.95, 0.95);
    draw_rounded_rect(cr, rect.x + 3, rect.y + 3, rect.width - 6, rect.height - 6, (rect.height - 6) / 2.0);
    cairo_fill(cr);

    // Chữ chỉ dẫn chuyển trang
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 36.0);

    std::string text = "Tiếp tục sang trang   ->";
    cairo_text_extents_t ext;
    cairo_text_extents(cr, text.c_str(), &ext);

    double tx = rect.x + (rect.width - ext.width) / 2.0;
    double ty = rect.y + (rect.height + ext.height) / 2.0 - 4;

    cairo_move_to(cr, tx, ty);
    cairo_show_text(cr, text.c_str());

    cairo_restore(cr);
}

} // namespace gach::adapters::ui
