#include "adapters/ui/minimal_components.hpp"
#include <cmath>
#include <cstdint>
#include <string>

namespace gach::adapters::ui {

void MinimalComponents::draw_rounded_rect(cairo_t* cr, double x, double y, double w, double h, double r) {
    cairo_new_sub_path(cr);
    cairo_arc(cr, x + w - r, y + r, r, -M_PI/2, 0);
    cairo_arc(cr, x + w - r, y + h - r, r, 0, M_PI/2);
    cairo_arc(cr, x + r, y + h - r, r, M_PI/2, M_PI);
    cairo_arc(cr, x + r, y + r, r, M_PI, 3*M_PI/2);
    cairo_close_path(cr);
}

void MinimalComponents::draw_utility_pill(cairo_t* cr, double x, double y, double w, double h,
                                         std::string_view label, UtilityIcon icon) {
    cairo_save(cr);

    // 1. Viền ngoài bo tròn dạng viên thuốc thanh mảnh 1.5px
    cairo_set_source_rgb(cr, 0.85, 0.85, 0.85);
    cairo_set_line_width(cr, 1.5);
    draw_rounded_rect(cr, x, y, w, h, h / 2.0);
    cairo_stroke(cr);

    // 2. Chữ nhãn nút (Label)
    cairo_set_source_rgb(cr, 0.35, 0.35, 0.35);
    cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 19.0);

    cairo_text_extents_t ext;
    cairo_text_extents(cr, std::string(label).c_str(), &ext);

    double text_x = x + 14.0;
    double text_y = y + (h + ext.height) / 2.0 - 2.0;

    cairo_move_to(cr, text_x, text_y);
    cairo_show_text(cr, std::string(label).c_str());

    // 3. Biểu tượng vector vẽ trực tiếp bằng Cairo
    double icon_cx = x + w - 20.0;
    double icon_cy = y + h / 2.0;

    switch (icon) {
        case UtilityIcon::CLOSE: {
            double s = 6.0;
            cairo_set_line_width(cr, 2.0);
            cairo_move_to(cr, icon_cx - s, icon_cy - s); cairo_line_to(cr, icon_cx + s, icon_cy + s);
            cairo_move_to(cr, icon_cx + s, icon_cy - s); cairo_line_to(cr, icon_cx - s, icon_cy + s);
            cairo_stroke(cr);
            break;
        }
        case UtilityIcon::BACK: {
            double s = 6.0;
            cairo_set_line_width(cr, 2.0);
            cairo_move_to(cr, icon_cx + s, icon_cy); cairo_line_to(cr, icon_cx - s, icon_cy);
            cairo_move_to(cr, icon_cx - s + 4, icon_cy - 4); cairo_line_to(cr, icon_cx - s, icon_cy); cairo_line_to(cr, icon_cx - s + 4, icon_cy + 4);
            cairo_stroke(cr);
            break;
        }
        case UtilityIcon::REPLAY: {
            // Biểu tượng loa phát âm thanh
            cairo_set_line_width(cr, 2.0);
            cairo_rectangle(cr, icon_cx - 6, icon_cy - 4, 4, 8);
            cairo_fill(cr);
            cairo_move_to(cr, icon_cx - 2, icon_cy - 4);
            cairo_line_to(cr, icon_cx + 4, icon_cy - 8);
            cairo_line_to(cr, icon_cx + 4, icon_cy + 8);
            cairo_line_to(cr, icon_cx - 2, icon_cy + 4);
            cairo_close_path(cr);
            cairo_fill(cr);
            break;
        }
        case UtilityIcon::MENU: {
            // Biểu tượng 3 gạch ngang
            cairo_set_line_width(cr, 2.0);
            cairo_move_to(cr, icon_cx - 6, icon_cy - 4); cairo_line_to(cr, icon_cx + 6, icon_cy - 4);
            cairo_move_to(cr, icon_cx - 6, icon_cy);     cairo_line_to(cr, icon_cx + 6, icon_cy);
            cairo_move_to(cr, icon_cx - 6, icon_cy + 4); cairo_line_to(cr, icon_cx + 6, icon_cy + 4);
            cairo_stroke(cr);
            break;
        }
        default:
            break;
    }

    cairo_restore(cr);
}

void MinimalComponents::draw_action_pill(cairo_t* cr, double x, double y, double w, double h,
                                        std::string_view text, ButtonStyle style) {
    cairo_save(cr);

    if (style == ButtonStyle::PRIMARY_SOLID) {
        // Nền đen tuyền dứt khoát (High-contrast CTA)
        cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
        draw_rounded_rect(cr, x, y, w, h, h / 2.0);
        cairo_fill(cr);

        cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    } else {
        // Nền trắng, viền đen sắc nét
        cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
        draw_rounded_rect(cr, x, y, w, h, h / 2.0);
        cairo_fill(cr);

        cairo_set_source_rgb(cr, 0.15, 0.15, 0.15);
        cairo_set_line_width(cr, 2.0);
        draw_rounded_rect(cr, x, y, w, h, h / 2.0);
        cairo_stroke(cr);

        cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    }

    cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 36.0);

    cairo_text_extents_t ext;
    cairo_text_extents(cr, std::string(text).c_str(), &ext);

    double tx = x + (w - ext.width) / 2.0;
    double ty = y + (h + ext.height) / 2.0 - 4.0;

    cairo_move_to(cr, tx, ty);
    cairo_show_text(cr, std::string(text).c_str());

    cairo_restore(cr);
}

void MinimalComponents::draw_choice_tile(cairo_t* cr, double x, double y, double w, double h,
                                        std::string_view text, ChoiceState state) {
    cairo_save(cr);

    double r = 18.0;

    if (state == ChoiceState::HINTED) {
        // Đảo màu Swiss Inversion: Nền đen chữ trắng
        cairo_set_source_rgb(cr, 0.05, 0.05, 0.05);
        draw_rounded_rect(cr, x, y, w, h, r);
        cairo_fill(cr);

        cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    } else if (state == ChoiceState::DISABLED) {
        // Trạng thái mờ nhẹ
        cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
        draw_rounded_rect(cr, x, y, w, h, r);
        cairo_fill(cr);

        cairo_set_source_rgb(cr, 0.75, 0.75, 0.75);
        cairo_set_line_width(cr, 1.5);
        draw_rounded_rect(cr, x, y, w, h, r);
        cairo_stroke(cr);

        cairo_set_source_rgb(cr, 0.65, 0.65, 0.65);
    } else {
        // Bình thường: Nền trắng, viền sắc nét 2px
        cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
        draw_rounded_rect(cr, x, y, w, h, r);
        cairo_fill(cr);

        cairo_set_source_rgb(cr, 0.15, 0.15, 0.15);
        cairo_set_line_width(cr, 2.0);
        draw_rounded_rect(cr, x, y, w, h, r);
        cairo_stroke(cr);

        cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    }

    cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 82.0);

    cairo_text_extents_t ext;
    cairo_text_extents(cr, std::string(text).c_str(), &ext);

    double tx = x + (w - ext.width) / 2.0 - ext.x_bearing;
    double ty = y + (h - ext.height) / 2.0 - ext.y_bearing;

    cairo_move_to(cr, tx, ty);
    cairo_show_text(cr, std::string(text).c_str());

    cairo_restore(cr);
}

void MinimalComponents::draw_phonics_slot(cairo_t* cr, double x, double y, double w, double h,
                                         std::string_view onset, std::string_view rhyme,
                                         bool missing_onset, bool completed, std::string_view full_word) {
    cairo_save(cr);

    // 1. Thẻ nền xám tro nhẹ nhàng
    cairo_set_source_rgb(cr, 0.98, 0.98, 0.98);
    draw_rounded_rect(cr, x, y, w, h, 16.0);
    cairo_fill(cr);

    cairo_set_source_rgb(cr, 0.82, 0.82, 0.82);
    cairo_set_line_width(cr, 1.5);
    draw_rounded_rect(cr, x, y, w, h, 16.0);
    cairo_stroke(cr);

    double center_y = y + h / 2.0;
    cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);

    if (completed) {
        // Từ hoàn chỉnh hiển thị nổi bật 72px ở giữa
        cairo_set_font_size(cr, 72.0);
        cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);

        cairo_text_extents_t ext;
        cairo_text_extents(cr, std::string(full_word).c_str(), &ext);

        double tx = x + (w - ext.width) / 2.0;
        double ty = center_y + ext.height / 2.0 - 4.0;
        cairo_move_to(cr, tx, ty);
        cairo_show_text(cr, std::string(full_word).c_str());
    } else {
        double slot_w = 110.0;
        double slot_h = 68.0;
        double slot_r = 12.0;

        cairo_set_font_size(cr, 66.0);
        cairo_text_extents_t letter_ext;

        if (missing_onset) {
            cairo_text_extents(cr, std::string(rhyme).c_str(), &letter_ext);

            double spacing = 24.0;
            double total_w = slot_w + spacing + letter_ext.width;
            double start_x = x + (w - total_w) / 2.0;

            double slot_x = start_x;
            double slot_y = center_y - slot_h / 2.0;

            // Slot trắng
            cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
            draw_rounded_rect(cr, slot_x, slot_y, slot_w, slot_h, slot_r);
            cairo_fill(cr);

            // Viền nét đứt
            const double dashes[] = {6.0, 4.0};
            cairo_set_dash(cr, dashes, 2, 0);
            cairo_set_source_rgb(cr, 0.4, 0.4, 0.4);
            cairo_set_line_width(cr, 2.0);
            draw_rounded_rect(cr, slot_x, slot_y, slot_w, slot_h, slot_r);
            cairo_stroke(cr);
            cairo_set_dash(cr, nullptr, 0, 0);

            // Dấu hỏi '?'
            cairo_set_source_rgb(cr, 0.55, 0.55, 0.55);
            cairo_set_font_size(cr, 42.0);
            cairo_text_extents_t q_ext;
            cairo_text_extents(cr, "?", &q_ext);
            cairo_move_to(cr, slot_x + (slot_w - q_ext.width) / 2.0, slot_y + (slot_h + q_ext.height) / 2.0 - 2.0);
            cairo_show_text(cr, "?");

            // Vần chữ
            cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
            cairo_set_font_size(cr, 66.0);
            double lx = start_x + slot_w + spacing;
            double ly = center_y + letter_ext.height / 2.0 - 4.0;
            cairo_move_to(cr, lx, ly);
            cairo_show_text(cr, std::string(rhyme).c_str());

        } else {
            cairo_text_extents(cr, std::string(onset).c_str(), &letter_ext);

            double spacing = 24.0;
            double total_w = letter_ext.width + spacing + slot_w;
            double start_x = x + (w - total_w) / 2.0;

            // Âm đầu
            cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
            cairo_set_font_size(cr, 66.0);
            double lx = start_x;
            double ly = center_y + letter_ext.height / 2.0 - 4.0;
            cairo_move_to(cr, lx, ly);
            cairo_show_text(cr, std::string(onset).c_str());

            // Slot bên phải
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
            cairo_move_to(cr, slot_x + (slot_w - q_ext.width) / 2.0, slot_y + (slot_h + q_ext.height) / 2.0 - 2.0);
            cairo_show_text(cr, "?");
        }
    }

    cairo_restore(cr);
}

void MinimalComponents::draw_page_tracker(cairo_t* cr, double center_x, double baseline_y,
                                         uint32_t current_page, uint32_t total_pages) {
    cairo_save(cr);
    cairo_set_source_rgb(cr, 0.45, 0.45, 0.45);
    cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 22.0);

    std::string text = std::to_string(current_page) + "  /  " + std::to_string(total_pages);
    cairo_text_extents_t ext;
    cairo_text_extents(cr, text.c_str(), &ext);

    cairo_move_to(cr, center_x - ext.width / 2.0, baseline_y);
    cairo_show_text(cr, text.c_str());

    cairo_restore(cr);
}

void MinimalComponents::draw_modal_dialog(cairo_t* cr, double screen_w, double screen_h,
                                         std::string_view title, std::string_view message,
                                         std::string_view primary_btn, std::string_view secondary_btn) {
    cairo_save(cr);

    // 1. Phủ mờ nền nhẹ E-Ink (50% translucent white)
    cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 0.85);
    cairo_rectangle(cr, 0, 0, screen_w, screen_h);
    cairo_fill(cr);

    // 2. Hộp thoại nổi ở chính giữa
    double dw = 760.0;
    double dh = 460.0;
    double dx = (screen_w - dw) / 2.0;
    double dy = (screen_h - dh) / 2.0;
    double r = 24.0;

    // Nền trắng tinh khiết
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    draw_rounded_rect(cr, dx, dy, dw, dh, r);
    cairo_fill(cr);

    // Viền đen đậm 3px
    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
    cairo_set_line_width(cr, 3.0);
    draw_rounded_rect(cr, dx, dy, dw, dh, r);
    cairo_stroke(cr);

    // 3. Tiêu đề hộp thoại
    cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 42.0);
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);

    cairo_text_extents_t t_ext;
    cairo_text_extents(cr, std::string(title).c_str(), &t_ext);
    cairo_move_to(cr, dx + (dw - t_ext.width) / 2.0, dy + 110.0);
    cairo_show_text(cr, std::string(title).c_str());

    // 4. Thông điệp
    cairo_set_font_size(cr, 30.0);
    cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_source_rgb(cr, 0.25, 0.25, 0.25);

    cairo_text_extents_t m_ext;
    cairo_text_extents(cr, std::string(message).c_str(), &m_ext);
    cairo_move_to(cr, dx + (dw - m_ext.width) / 2.0, dy + 190.0);
    cairo_show_text(cr, std::string(message).c_str());

    // 5. Cặp nút hành động
    double btn_w = 300.0;
    double btn_h = 80.0;
    double btn_y = dy + dh - 120.0;
    double gap = 30.0;
    double total_btns_w = 2 * btn_w + gap;
    double start_btn_x = dx + (dw - total_btns_w) / 2.0;

    // Nút phụ (Secondary) bên trái
    draw_action_pill(cr, start_btn_x, btn_y, btn_w, btn_h, secondary_btn, ButtonStyle::SECONDARY_OUTLINE);

    // Nút chính (Primary) bên phải
    draw_action_pill(cr, start_btn_x + btn_w + gap, btn_y, btn_w, btn_h, primary_btn, ButtonStyle::PRIMARY_SOLID);

    cairo_restore(cr);
}

} // namespace gach::adapters::ui
