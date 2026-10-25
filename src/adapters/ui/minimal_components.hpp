#pragma once

#include <cairo.h>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace gach::adapters::ui {

/**
 * @brief GACH Minimalism Design System (GMDS) Component Kit.
 * Thư viện các thành phần giao diện chuẩn phong cách Minimalism tinh tế cho màn hình E-Ink.
 */
class MinimalComponents {
public:
    // Helper: Vẽ hình chữ nhật bo góc toán học hoàn hảo
    static void draw_rounded_rect(cairo_t* cr, double x, double y, double w, double h, double r);

    // 1. NÚT TIỆN ÍCH ĐỈNH MÀN HÌNH (Floating Utility Pill)
    // Loại icon hỗ trợ: CLOSE (✕), BACK (←), REPLAY (🔊), MENU (☰), SETTINGS (⚙)
    enum class UtilityIcon { NONE, CLOSE, BACK, REPLAY, MENU, SETTINGS };
    static void draw_utility_pill(cairo_t* cr, double x, double y, double w, double h,
                                  std::string_view label, UtilityIcon icon = UtilityIcon::NONE,
                                  bool disabled = false);

    // 2. NÚT HÀNH ĐỘNG CHÍNH / PHỤ (Action Pill CTA)
    enum class ButtonStyle {
        PRIMARY_SOLID,   // Nền đen tuyền, chữ trắng sắc nét (Primary CTA)
        SECONDARY_OUTLINE // Nền trắng, viền đen mảnh 2px, chữ đen (Secondary)
    };
    static void draw_action_pill(cairo_t* cr, double x, double y, double w, double h,
                                 std::string_view text, ButtonStyle style = ButtonStyle::PRIMARY_SOLID);

    // 3. MẢNH GHÉP LỰA CHỌN CHỮ CÁI (Choice Tile)
    enum class ChoiceState {
        NORMAL,   // Nền trắng, viền mảnh 2px, chữ đen to 82px
        HINTED,   // Đảo màu: Nền đen tuyền, chữ trắng nổi bật (Swiss Inversion)
        DISABLED  // Mờ nhẹ cho nhịp chờ
    };
    static void draw_choice_tile(cairo_t* cr, double x, double y, double w, double h,
                                 std::string_view text, ChoiceState state = ChoiceState::NORMAL);

    // 4. KHAY TIÊU ĐIỂM ÂM VỊ (Phonics Focus Tray)
    // missing_onset = true: [ ? ] + rhyme
    // missing_onset = false: onset + [ ? ]
    static void draw_phonics_slot(cairo_t* cr, double x, double y, double w, double h,
                                  std::string_view onset, std::string_view rhyme,
                                  bool missing_onset, bool completed, std::string_view full_word);

    // 5. CHỈ BÁO TIẾN TRÌNH TRANG (Page Indicator)
    static void draw_page_tracker(cairo_t* cr, double center_x, double baseline_y,
                                  uint32_t current_page, uint32_t total_pages);

    // 6. HỘP THOẠI XÁC NHẬN TỐI GIẢN (Minimalist Modal Dialog)
    static void draw_modal_dialog(cairo_t* cr, double screen_w, double screen_h,
                                  std::string_view title, std::string_view message,
                                  std::string_view primary_btn, std::string_view secondary_btn);

    // 7. HỘP THOẠI MỤC LỤC TRUYỆN (Table of Contents Modal)
    static void draw_toc_overlay(cairo_t* cr, int screen_w, int screen_h, uint32_t active_page,
                                 const std::vector<std::string>& words);

    // 8. HỘP THOẠI CÀI ĐẶT ỨNG DỤNG (Settings Modal)
    static void draw_settings_overlay(cairo_t* cr, int screen_w, int screen_h, bool auto_hint);
};

} // namespace gach::adapters::ui
