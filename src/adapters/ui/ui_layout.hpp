#pragma once

#include <array>
#include <cstdint>

namespace gach::adapters::ui {

enum class OverlayMode : uint8_t {
    NONE = 0,
    MENU_TOC = 1,
    SETTINGS = 2
};

struct Rect {
    int x{0};
    int y{0};
    int width{0};
    int height{0};

    [[nodiscard]] constexpr bool contains(int px, int py) const noexcept {
        return px >= x && px <= (x + width) && py >= y && py <= (y + height);
    }
};

/**
 * @brief Tính toán bố cục màn hình và hitboxes chuẩn phong cách Tối Giản (Minimalism).
 * Hỗ trợ các nút điều hướng thường dùng: Quay lại (Back), Mục lục (Menu), Cài đặt (Settings), Thoát (Exit).
 */
class UILayout {
public:
    UILayout() noexcept = default;
    void update_resolution(int screen_w, int screen_h) noexcept;

    // Các thành phần trang cơ bản
    [[nodiscard]] Rect image_rect() const noexcept { return m_image_rect; }
    [[nodiscard]] Rect text_rect() const noexcept { return m_text_rect; }
    [[nodiscard]] Rect phonics_word_rect() const noexcept { return m_phonics_word_rect; }
    [[nodiscard]] Rect choice_rect(uint8_t index) const noexcept;
    [[nodiscard]] Rect next_button_rect() const noexcept { return m_next_btn_rect; }

    // Thanh điều hướng đỉnh (Top Bar Navigation)
    [[nodiscard]] Rect back_button_rect() const noexcept { return m_back_btn_rect; }
    [[nodiscard]] Rect menu_button_rect() const noexcept { return m_menu_btn_rect; }
    [[nodiscard]] Rect settings_button_rect() const noexcept { return m_settings_btn_rect; }
    [[nodiscard]] Rect exit_button_rect() const noexcept { return m_exit_btn_rect; }

    // Hit-testing trang cơ bản
    [[nodiscard]] int hit_test_choice(int px, int py, uint8_t choice_count) const noexcept;
    [[nodiscard]] bool hit_test_next(int px, int py) const noexcept;
    [[nodiscard]] bool hit_test_back(int px, int py) const noexcept;
    [[nodiscard]] bool hit_test_menu(int px, int py) const noexcept;
    [[nodiscard]] bool hit_test_settings(int px, int py) const noexcept;
    [[nodiscard]] bool hit_test_exit(int px, int py) const noexcept;

    // Hit-testing Mục Lục (TOC Modal Overlay)
    [[nodiscard]] bool hit_test_toc_close(int px, int py) const noexcept;
    [[nodiscard]] int hit_test_toc_item(int px, int py) const noexcept; // Trả về 1..16 hoặc -1

    // Hit-testing Cài Đặt (Settings Modal Overlay)
    [[nodiscard]] bool hit_test_settings_close(int px, int py) const noexcept;
    [[nodiscard]] bool hit_test_settings_toggle_hint(int px, int py) const noexcept;
    [[nodiscard]] bool hit_test_settings_reset(int px, int py) const noexcept;

private:
    int m_screen_width{1072};
    int m_screen_height{1448};

    // Trang chính
    Rect m_image_rect{};
    Rect m_text_rect{};
    Rect m_phonics_word_rect{};
    std::array<Rect, 4> m_choice_rects{};
    Rect m_next_btn_rect{};

    // Thanh điều hướng
    Rect m_back_btn_rect{};
    Rect m_menu_btn_rect{};
    Rect m_settings_btn_rect{};
    Rect m_exit_btn_rect{};

    // Mục lục (TOC) Overlay
    Rect m_toc_card_rect{};
    Rect m_toc_close_btn_rect{};
    std::array<Rect, 16> m_toc_item_rects{};

    // Cài đặt (Settings) Overlay
    Rect m_settings_card_rect{};
    Rect m_settings_close_btn_rect{};
    Rect m_settings_toggle_hint_rect{};
    Rect m_settings_reset_btn_rect{};
    Rect m_settings_done_btn_rect{};
};

} // namespace gach::adapters::ui
