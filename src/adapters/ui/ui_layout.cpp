#include "adapters/ui/ui_layout.hpp"
#include <algorithm>

namespace gach::adapters::ui {

void UILayout::update_resolution(int screen_w, int screen_h) noexcept {
    m_screen_width = screen_w > 0 ? screen_w : 1072;
    m_screen_height = screen_h > 0 ? screen_h : 1448;

    // 1. Khung tranh (Chiếm khoảng 50% - 55% chiều cao trên cùng)
    int img_h = static_cast<int>(m_screen_height * 0.52);
    m_image_rect = Rect{ 10, 10, m_screen_width - 20, img_h };

    // 2. Vùng lời dẫn truyện (Text Area)
    int text_y = img_h + 20;
    int text_h = static_cast<int>(m_screen_height * 0.18);
    m_text_rect = Rect{ 40, text_y, m_screen_width - 80, text_h };

    // 3. Vùng từ vựng mục tiêu với vần khuyết (Visual Phonics)
    int phonics_y = text_y + text_h + 10;
    int phonics_h = static_cast<int>(m_screen_height * 0.08);
    m_phonics_word_rect = Rect{ 40, phonics_y, m_screen_width - 80, phonics_h };

    // 4. Các mảnh ghép lựa chọn (Choices) - Hitbox to cho tay trẻ mầm non
    int choices_y = phonics_y + phonics_h + 15;
    int choice_w = std::max(120, static_cast<int>(m_screen_width * 0.22));
    int choice_h = std::max(90, static_cast<int>(m_screen_height * 0.09));
    int spacing = (m_screen_width - 80 - (3 * choice_w)) / 2;
    if (spacing < 20) spacing = 20;

    for (int i = 0; i < 3; ++i) {
        int cx = 40 + i * (choice_w + spacing);
        m_choice_rects[i] = Rect{ cx, choices_y, choice_w, choice_h };
    }
    m_choice_rects[3] = Rect{ 0, 0, 0, 0 }; // Dự phòng

    // 5. Nút mũi tên sang trang tiếp theo (Góc phải dưới)
    int btn_size = std::max(110, static_cast<int>(m_screen_width * 0.16));
    int btn_x = m_screen_width - btn_size - 40;
    int btn_y = m_screen_height - btn_size - 30;
    m_next_btn_rect = Rect{ btn_x, btn_y, btn_size, btn_size };
}

Rect UILayout::choice_rect(uint8_t index) const noexcept {
    if (index < m_choice_rects.size()) {
        return m_choice_rects[index];
    }
    return Rect{ 0, 0, 0, 0 };
}

int UILayout::hit_test_choice(int px, int py, uint8_t choice_count) const noexcept {
    uint8_t limit = std::min(choice_count, static_cast<uint8_t>(m_choice_rects.size()));
    for (uint8_t i = 0; i < limit; ++i) {
        if (m_choice_rects[i].contains(px, py)) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

bool UILayout::hit_test_next(int px, int py) const noexcept {
    return m_next_btn_rect.contains(px, py);
}

} // namespace gach::adapters::ui