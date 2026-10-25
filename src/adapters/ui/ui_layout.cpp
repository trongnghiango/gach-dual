#include "adapters/ui/ui_layout.hpp"
#include <algorithm>

namespace gach::adapters::ui {

void UILayout::update_resolution(int screen_w, int screen_h) noexcept {
    m_screen_width = screen_w > 0 ? screen_w : 1072;
    m_screen_height = screen_h > 0 ? screen_h : 1448;

    double scale_x = static_cast<double>(m_screen_width) / 1072.0;
    double scale_y = static_cast<double>(m_screen_height) / 1448.0;
    double s = std::min(scale_x, scale_y);

    int margin_x = static_cast<int>(32 * s);
    int content_w = m_screen_width - 2 * margin_x;

    // 1. Khung tranh minh họa (Picture Frame) - Tỷ lệ 44% chiều cao màn hình
    int img_y = static_cast<int>(45 * s);
    int img_h = static_cast<int>(m_screen_height * 0.44);
    m_image_rect = Rect{ margin_x, img_y, content_w, img_h };

    // 2. Vùng lời dẫn truyện (Story Lead Text) - Rộng rãi cho 2-3 dòng chữ to
    int text_y = img_y + img_h + static_cast<int>(20 * s);
    int text_h = static_cast<int>(140 * s);
    m_text_rect = Rect{ margin_x + static_cast<int>(16 * s), text_y, content_w - static_cast<int>(32 * s), text_h };

    // 3. Vùng thẻ âm vị (Visual Phonics Card)
    int phonics_y = text_y + text_h + static_cast<int>(15 * s);
    int phonics_h = static_cast<int>(100 * s);
    m_phonics_word_rect = Rect{ margin_x, phonics_y, content_w, phonics_h };

    // 4. Các mảnh ghép lựa chọn (3 Choice Cards) - Rộng rãi cho tay trẻ
    int choices_y = phonics_y + phonics_h + static_cast<int>(20 * s);
    int choice_h = static_cast<int>(130 * s);
    int choice_w = static_cast<int>(280 * s);
    int total_choices_w = 3 * choice_w;
    int spacing = (content_w - total_choices_w) / 2;
    if (spacing < 10) {
        choice_w = (content_w - 40) / 3;
        spacing = 20;
    }

    for (int i = 0; i < 3; ++i) {
        int cx = margin_x + i * (choice_w + spacing);
        m_choice_rects[i] = Rect{ cx, choices_y, choice_w, choice_h };
    }
    m_choice_rects[3] = Rect{ 0, 0, 0, 0 };

    // 5. Nút Next Page (Pill Button chuyển trang đặt ở đáy, giữa màn hình)
    int btn_w = static_cast<int>(500 * s);
    int btn_h = static_cast<int>(95 * s);
    int btn_x = (m_screen_width - btn_w) / 2;
    int btn_y = choices_y + choice_h + static_cast<int>(20 * s);
    if (btn_y + btn_h > m_screen_height - 10) {
        btn_y = m_screen_height - btn_h - 15;
    }
    m_next_btn_rect = Rect{ btn_x, btn_y, btn_w, btn_h };
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
