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
                                         std::string_view label, UtilityIcon icon, bool disabled) {
    cairo_save(cr);

    // 1. Viền ngoài bo tròn dạng viên thuốc thanh mảnh 1.5px
    if (disabled) {
        cairo_set_source_rgb(cr, 0.90, 0.90, 0.90);
    } else {
        cairo_set_source_rgb(cr, 0.85, 0.85, 0.85);
    }
    cairo_set_line_width(cr, 1.5);
    draw_rounded_rect(cr, x, y, w, h, h / 2.0);
    cairo_stroke(cr);

    // 2. Chữ nhãn nút (Label)
    if (disabled) {
        cairo_set_source_rgb(cr, 0.70, 0.70, 0.70);
    } else {
        cairo_set_source_rgb(cr, 0.35, 0.35, 0.35);
    }
    cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 19.0);

    cairo_text_extents_t ext;
    cairo_text_extents(cr, std::string(label).c_str(), &ext);

    double text_x = x + 12.0;
    double text_y = y + (h + ext.height) / 2.0 - 2.0;

    cairo_move_to(cr, text_x, text_y);
    cairo_show_text(cr, std::string(label).c_str());

    // 3. Biểu tượng vector vẽ trực tiếp bằng Cairo
    double icon_cx = x + w - 18.0;
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
            cairo_move_to(cr, icon_cx - 6, icon_cy - 5); cairo_line_to(cr, icon_cx + 6, icon_cy - 5);
            cairo_move_to(cr, icon_cx - 6, icon_cy);     cairo_line_to(cr, icon_cx + 6, icon_cy);
            cairo_move_to(cr, icon_cx - 6, icon_cy + 5); cairo_line_to(cr, icon_cx + 6, icon_cy + 5);
            cairo_stroke(cr);
            break;
        }
        case UtilityIcon::SETTINGS: {
            // Biểu tượng bánh răng cài đặt ⚙ tối giản
            cairo_set_line_width(cr, 1.8);
            cairo_new_sub_path(cr);
            cairo_arc(cr, icon_cx, icon_cy, 6.0, 0, 2 * M_PI);
            cairo_stroke(cr);
            cairo_new_sub_path(cr);
            cairo_arc(cr, icon_cx, icon_cy, 2.2, 0, 2 * M_PI);
            cairo_fill(cr);
            for (int i = 0; i < 6; ++i) {
                double a = i * (M_PI / 3.0);
                cairo_move_to(cr, icon_cx + 5.5 * std::cos(a), icon_cy + 5.5 * std::sin(a));
                cairo_line_to(cr, icon_cx + 9.5 * std::cos(a), icon_cy + 9.5 * std::sin(a));
            }
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

void MinimalComponents::draw_toc_overlay(cairo_t* cr, int screen_w, int screen_h, uint32_t active_page,
                                         const std::vector<std::string>& words) {
    cairo_save(cr);

    // 1. Phủ mờ nhẹ nhàng nền trang (85% white)
    cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 0.90);
    cairo_rectangle(cr, 0, 0, screen_w, screen_h);
    cairo_fill(cr);

    // 2. Thẻ Card mục lục lớn ở giữa
    double card_w = 940.0;
    double card_h = 1140.0;
    double card_x = (screen_w - card_w) / 2.0;
    double card_y = (screen_h - card_h) / 2.0;

    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    draw_rounded_rect(cr, card_x, card_y, card_w, card_h, 24.0);
    cairo_fill(cr);

    cairo_set_source_rgb(cr, 0.15, 0.15, 0.15);
    cairo_set_line_width(cr, 2.5);
    draw_rounded_rect(cr, card_x, card_y, card_w, card_h, 24.0);
    cairo_stroke(cr);

    // Tiêu đề Mục Lục
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 38.0);
    cairo_move_to(cr, card_x + 48.0, card_y + 68.0);
    cairo_show_text(cr, "Mục Lục 16 Trang Truyện");

    // Nút Đóng (✕)
    double close_x = card_x + card_w - 110.0;
    double close_y = card_y + 36.0;
    draw_utility_pill(cr, close_x, close_y, 75.0, 36.0, "Đóng", UtilityIcon::CLOSE);

    // 3. Grid 16 ô đại diện cho 16 trang (4 cột x 4 hàng)
    double grid_start_x = card_x + 48.0;
    double grid_start_y = card_y + 115.0;
    double item_gap_x = 22.0;
    double item_gap_y = 22.0;
    double item_w = (card_w - 96.0 - 3.0 * item_gap_x) / 4.0;
    double item_h = 220.0;

    size_t total = std::min(words.size(), static_cast<size_t>(16));
    for (size_t i = 0; i < total; ++i) {
        int row = static_cast<int>(i / 4);
        int col = static_cast<int>(i % 4);
        double ix = grid_start_x + col * (item_w + item_gap_x);
        double iy = grid_start_y + row * (item_h + item_gap_y);

        bool is_current = (i + 1 == active_page);

        if (is_current) {
            cairo_set_source_rgb(cr, 0.05, 0.05, 0.05);
            draw_rounded_rect(cr, ix, iy, item_w, item_h, 16.0);
            cairo_fill(cr);
        } else {
            cairo_set_source_rgb(cr, 0.98, 0.98, 0.98);
            draw_rounded_rect(cr, ix, iy, item_w, item_h, 16.0);
            cairo_fill(cr);

            cairo_set_source_rgb(cr, 0.85, 0.85, 0.85);
            cairo_set_line_width(cr, 1.5);
            draw_rounded_rect(cr, ix, iy, item_w, item_h, 16.0);
            cairo_stroke(cr);
        }

        // Nhãn trang
        if (is_current) {
            cairo_set_source_rgb(cr, 0.8, 0.8, 0.8);
        } else {
            cairo_set_source_rgb(cr, 0.5, 0.5, 0.5);
        }
        cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
        cairo_set_font_size(cr, 20.0);
        std::string p_label = "Trang " + std::to_string(i + 1);
        cairo_move_to(cr, ix + 16.0, iy + 36.0);
        cairo_show_text(cr, p_label.c_str());

        // Từ ngữ mục tiêu to rõ ở giữa
        if (is_current) {
            cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
        } else {
            cairo_set_source_rgb(cr, 0.05, 0.05, 0.05);
        }
        cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
        cairo_set_font_size(cr, 46.0);
        cairo_text_extents_t w_ext;
        cairo_text_extents(cr, words[i].c_str(), &w_ext);
        cairo_move_to(cr, ix + (item_w - w_ext.width) / 2.0, iy + item_h / 2.0 + 35.0);
        cairo_show_text(cr, words[i].c_str());
    }

    cairo_restore(cr);
}

void MinimalComponents::draw_settings_overlay(cairo_t* cr, int screen_w, int screen_h, bool auto_hint) {
    cairo_save(cr);

    // 1. Phủ mờ nền trang
    cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 0.90);
    cairo_rectangle(cr, 0, 0, screen_w, screen_h);
    cairo_fill(cr);

    // 2. Thẻ Card Cài Đặt
    double card_w = 780.0;
    double card_h = 600.0;
    double card_x = (screen_w - card_w) / 2.0;
    double card_y = (screen_h - card_h) / 2.0;

    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    draw_rounded_rect(cr, card_x, card_y, card_w, card_h, 24.0);
    cairo_fill(cr);

    cairo_set_source_rgb(cr, 0.15, 0.15, 0.15);
    cairo_set_line_width(cr, 2.5);
    draw_rounded_rect(cr, card_x, card_y, card_w, card_h, 24.0);
    cairo_stroke(cr);

    // Tiêu đề
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 38.0);
    cairo_move_to(cr, card_x + 48.0, card_y + 68.0);
    cairo_show_text(cr, "Cài Đặt Ứng Dụng");

    // Nút Đóng (✕)
    double close_x = card_x + card_w - 110.0;
    double close_y = card_y + 36.0;
    draw_utility_pill(cr, close_x, close_y, 75.0, 36.0, "Đóng", UtilityIcon::CLOSE);

    // Mục 1: Gợi ý dẫn lối tự động sau 6s
    double item1_y = card_y + 130.0;
    cairo_set_source_rgb(cr, 0.98, 0.98, 0.98);
    draw_rounded_rect(cr, card_x + 36.0, item1_y, card_w - 72.0, 90.0, 16.0);
    cairo_fill(cr);
    cairo_set_source_rgb(cr, 0.85, 0.85, 0.85);
    cairo_set_line_width(cr, 1.5);
    draw_rounded_rect(cr, card_x + 36.0, item1_y, card_w - 72.0, 90.0, 16.0);
    cairo_stroke(cr);

    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
    cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 26.0);
    cairo_move_to(cr, card_x + 60.0, item1_y + 55.0);
    cairo_show_text(cr, "Gợi ý tự động sau 6 giây");

    // Nút toggle [ BẬT ] / [ TẮT ]
    double tog_w = 110.0;
    double tog_h = 50.0;
    double tog_x = card_x + card_w - 36.0 - 24.0 - tog_w;
    double tog_y = item1_y + 20.0;

    if (auto_hint) {
        cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
        draw_rounded_rect(cr, tog_x, tog_y, tog_w, tog_h, 25.0);
        cairo_fill(cr);
        cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
        cairo_set_font_size(cr, 22.0);
        cairo_move_to(cr, tog_x + 32.0, tog_y + 32.0);
        cairo_show_text(cr, "BẬT");
    } else {
        cairo_set_source_rgb(cr, 0.85, 0.85, 0.85);
        cairo_set_line_width(cr, 2.0);
        draw_rounded_rect(cr, tog_x, tog_y, tog_w, tog_h, 25.0);
        cairo_stroke(cr);
        cairo_set_source_rgb(cr, 0.4, 0.4, 0.4);
        cairo_set_font_size(cr, 22.0);
        cairo_move_to(cr, tog_x + 32.0, tog_y + 32.0);
        cairo_show_text(cr, "TẮT");
    }

    // Mục 2: Bắt đầu lại truyện từ Trang 1
    double item2_y = item1_y + 110.0;
    cairo_set_source_rgb(cr, 0.98, 0.98, 0.98);
    draw_rounded_rect(cr, card_x + 36.0, item2_y, card_w - 72.0, 90.0, 16.0);
    cairo_fill(cr);
    cairo_set_source_rgb(cr, 0.85, 0.85, 0.85);
    cairo_set_line_width(cr, 1.5);
    draw_rounded_rect(cr, card_x + 36.0, item2_y, card_w - 72.0, 90.0, 16.0);
    cairo_stroke(cr);

    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
    cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 26.0);
    cairo_move_to(cr, card_x + 60.0, item2_y + 55.0);
    cairo_show_text(cr, "Bắt đầu lại từ Trang 1");

    double rst_w = 170.0;
    double rst_h = 50.0;
    double rst_x = card_x + card_w - 36.0 - 24.0 - rst_w;
    double rst_y = item2_y + 20.0;

    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    draw_rounded_rect(cr, rst_x, rst_y, rst_w, rst_h, 25.0);
    cairo_fill(cr);
    cairo_set_source_rgb(cr, 0.2, 0.2, 0.2);
    cairo_set_line_width(cr, 2.0);
    draw_rounded_rect(cr, rst_x, rst_y, rst_w, rst_h, 25.0);
    cairo_stroke(cr);

    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
    cairo_set_font_size(cr, 22.0);
    cairo_move_to(cr, rst_x + 22.0, rst_y + 32.0);
    cairo_show_text(cr, "Đọc lại từ đầu");

    // Nút Hoàn tất
    double done_w = 320.0;
    double done_h = 75.0;
    double done_x = card_x + (card_w - done_w) / 2.0;
    double done_y = card_y + card_h - 110.0;

    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    draw_rounded_rect(cr, done_x, done_y, done_w, done_h, done_h / 2.0);
    cairo_fill(cr);

    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_set_font_size(cr, 28.0);
    cairo_move_to(cr, done_x + 105.0, done_y + 47.0);
    cairo_show_text(cr, "Hoàn tất");

    cairo_restore(cr);
}

} // namespace gach::adapters::ui
