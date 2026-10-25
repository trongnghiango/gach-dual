#include "adapters/ui/ui_layout.hpp"
#include "adapters/ui/ink_flex.hpp"
#include <algorithm>

namespace gach::adapters::ui {

void UILayout::update_resolution(int screen_w, int screen_h) noexcept {
    m_screen_width = screen_w > 0 ? screen_w : 1072;
    m_screen_height = screen_h > 0 ? screen_h : 1448;

    double scale = static_cast<double>(m_screen_width) / 1072.0;

    // Khởi tạo cây bố cục InkFlex Column
    auto root = make_column("page_root");
    root->set_size(m_screen_width, m_screen_height);
    root->set_padding(Edges::symmetric(static_cast<int>(10 * scale), static_cast<int>(34 * scale)));
    root->set_gap(static_cast<int>(18 * scale));

    // 1. Nút Thoát ứng dụng (Absolute Positioning, Top-Right, zIndex = 10)
    int exit_w = static_cast<int>(140 * scale);
    int exit_h = static_cast<int>(42 * scale);
    auto exit_btn = make_item("exit_btn");
    exit_btn->set_position(PositionType::Absolute);
    exit_btn->set_top(static_cast<int>(10 * scale));
    exit_btn->set_right(static_cast<int>(34 * scale));
    exit_btn->set_size(exit_w, exit_h);
    exit_btn->set_z_index(10);
    root->add_child(exit_btn);

    // 2. Vùng Header / Chỉ báo số trang (Relative, fixed height ~36px)
    auto header = make_item("header");
    header->set_height(static_cast<int>(36 * scale));
    root->add_child(header);

    // 3. Khung Tranh Minh Họa (Hero Picture Frame)
    // Tự động co giãn (flex_grow = 1.0f) chiếm trọn phần không gian thị giác đẹp nhất!
    auto hero = make_item("hero_image");
    hero->set_flex_grow(1.0f);
    hero->set_min_height(static_cast<int>(m_screen_height * 0.35));
    hero->set_max_height(static_cast<int>(m_screen_height * 0.46));
    root->add_child(hero);

    // 4. Lời Dẫn Truyện (Story Lead Text Card)
    // Tự động tính toán theo 2 dòng văn bản chuẩn
    int line_h = static_cast<int>(62 * scale);
    auto text_card = make_item("story_text");
    text_card->set_height(2 * line_h + static_cast<int>(16 * scale));
    root->add_child(text_card);

    // 5. Khay Gỗ Montessori Âm Vị (Visual Phonics Tray)
    auto phonics_tray = make_item("phonics_tray");
    phonics_tray->set_height(static_cast<int>(108 * scale));
    root->add_child(phonics_tray);

    // 6. Vùng Tương Tác: Khối Gỗ Chữ Cái (Row Flexbox)
    auto choices_row = make_row("choices_row");
    choices_row->set_height(static_cast<int>(135 * scale));
    choices_row->set_gap(static_cast<int>(20 * scale));
    choices_row->set_justify(JustifyContent::SpaceBetween);

    for (int i = 0; i < 3; ++i) {
        auto choice = make_item("choice_" + std::to_string(i));
        choice->set_flex_grow(1.0f);
        choices_row->add_child(choice);
    }
    root->add_child(choices_row);

    // Tính toán bố cục InkFlex đệ quy (Toán học chống chồng lấn 100%)
    root->layout(0, 0, m_screen_width, m_screen_height);

    // Gán kết quả tính toán vào các vùng Rect
    m_exit_btn_rect = Rect{ exit_btn->computed_x, exit_btn->computed_y, exit_btn->computed_w, exit_btn->computed_h };
    m_image_rect = Rect{ hero->computed_x, hero->computed_y, hero->computed_w, hero->computed_h };
    m_text_rect = Rect{ text_card->computed_x, text_card->computed_y, text_card->computed_w, text_card->computed_h };
    m_phonics_word_rect = Rect{ phonics_tray->computed_x, phonics_tray->computed_y, phonics_tray->computed_w, phonics_tray->computed_h };

    for (int i = 0; i < 3; ++i) {
        auto c = choices_row->children[i];
        m_choice_rects[i] = Rect{ c->computed_x, c->computed_y, c->computed_w, c->computed_h };
    }
    m_choice_rects[3] = Rect{ 0, 0, 0, 0 };

    // Nút chuyển trang (Next Button) nằm ở cùng vùng tầng tương tác, căn giữa màn hình
    int next_w = static_cast<int>(520 * scale);
    int next_h = static_cast<int>(100 * scale);
    int next_x = (m_screen_width - next_w) / 2;
    int next_y = choices_row->computed_y + (choices_row->computed_h - next_h) / 2;
    m_next_btn_rect = Rect{ next_x, next_y, next_w, next_h };
}

bool UILayout::hit_test_exit(int px, int py) const noexcept {
    return m_exit_btn_rect.contains(px, py);
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
