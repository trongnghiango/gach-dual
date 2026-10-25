#pragma once

#include <array>
#include <cstdint>

namespace gach::adapters::ui {

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
 * @brief Tính toán bố cục màn hình và hitboxes tối ưu cho ngón tay trẻ 3-6 tuổi.
 * Hỗ trợ PW3 (1072x1448 portrait), PW2 (758x1024), hoặc Desktop Window.
 */
class UILayout {
public:
    UILayout() noexcept = default;
    void update_resolution(int screen_w, int screen_h) noexcept;

    [[nodiscard]] Rect image_rect() const noexcept { return m_image_rect; }
    [[nodiscard]] Rect text_rect() const noexcept { return m_text_rect; }
    [[nodiscard]] Rect phonics_word_rect() const noexcept { return m_phonics_word_rect; }
    [[nodiscard]] Rect choice_rect(uint8_t index) const noexcept;
    [[nodiscard]] Rect next_button_rect() const noexcept { return m_next_btn_rect; }
    [[nodiscard]] Rect exit_button_rect() const noexcept { return m_exit_btn_rect; }

    [[nodiscard]] int hit_test_choice(int px, int py, uint8_t choice_count) const noexcept;
    [[nodiscard]] bool hit_test_next(int px, int py) const noexcept;
    [[nodiscard]] bool hit_test_exit(int px, int py) const noexcept;

private:
    int m_screen_width{1072};
    int m_screen_height{1448};
    Rect m_image_rect{};
    Rect m_text_rect{};
    Rect m_phonics_word_rect{};
    std::array<Rect, 4> m_choice_rects{};
    Rect m_next_btn_rect{};
    Rect m_exit_btn_rect{};
};

} // namespace gach::adapters::ui