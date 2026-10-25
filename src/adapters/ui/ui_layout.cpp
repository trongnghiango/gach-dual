#include "adapters/ui/ui_layout.hpp"
#include "adapters/ui/ink_flex.hpp"
#include <algorithm>

namespace gach::adapters::ui {

void UILayout::update_resolution(int screen_w, int screen_h) noexcept {
    m_screen_width = screen_w > 0 ? screen_w : 1072;
    m_screen_height = screen_h > 0 ? screen_h : 1448;

    double scale = static_cast<double>(m_screen_width) / 1072.0;

    // 1. THANH ĐIỀU HƯỚNG ĐỈNH (TOP NAVIGATION BAR)
    int btn_h = static_cast<int>(38 * scale);
    int top_y = static_cast<int>(24 * scale);
    int margin_x = static_cast<int>(48 * scale);

    // Nút Quay lại (←)
    int back_w = static_cast<int>(118 * scale);
    m_back_btn_rect = Rect{ margin_x, top_y, back_w, btn_h };

    // Nút Mục lục (☰)
    int menu_w = static_cast<int>(115 * scale);
    int menu_x = m_back_btn_rect.x + m_back_btn_rect.width + static_cast<int>(14 * scale);
    m_menu_btn_rect = Rect{ menu_x, top_y, menu_w, btn_h };

    // Nút Thoát (✕)
    int exit_w = static_cast<int>(95 * scale);
    int exit_x = m_screen_width - margin_x - exit_w;
    m_exit_btn_rect = Rect{ exit_x, top_y, exit_w, btn_h };

    // Nút Cài đặt (⚙)
    int settings_w = static_cast<int>(115 * scale);
    int settings_x = m_exit_btn_rect.x - static_cast<int>(14 * scale) - settings_w;
    m_settings_btn_rect = Rect{ settings_x, top_y, settings_w, btn_h };

    // 2. CÂY BỐ CỤC NỘI DUNG CHÍNH (INKFLEX COLUMN)
    auto root = make_column("page_root");
    root->set_size(m_screen_width, m_screen_height);
    root->set_padding(Edges{
        static_cast<int>(24 * scale),
        margin_x,
        static_cast<int>(36 * scale),
        margin_x
    });
    root->set_gap(static_cast<int>(24 * scale));

    // Khoảng đệm cho Header
    auto header = make_item("header");
    header->set_height(static_cast<int>(38 * scale));
    root->add_child(header);

    // Khung Tranh Minh Họa Ehon (Co giãn tự động flex_grow = 1.0f)
    auto hero = make_item("hero_image");
    hero->set_flex_grow(1.0f);
    hero->set_min_height(static_cast<int>(m_screen_height * 0.38));
    hero->set_max_height(static_cast<int>(m_screen_height * 0.46));
    root->add_child(hero);

    // Lời Dẫn Truyện (Typography 2 dòng)
    int line_h = static_cast<int>(56 * scale);
    auto text_card = make_item("story_text");
    text_card->set_height(2 * line_h + static_cast<int>(20 * scale));
    root->add_child(text_card);

    // Thẻ Tiêu Điểm Ghép Vần
    auto phonics_tray = make_item("phonics_tray");
    phonics_tray->set_height(static_cast<int>(110 * scale));
    root->add_child(phonics_tray);

    // 3 Mảnh Ghép Lựa Chọn
    auto choices_row = make_row("choices_row");
    choices_row->set_height(static_cast<int>(145 * scale));
    choices_row->set_gap(static_cast<int>(26 * scale));
    choices_row->set_justify(JustifyContent::SpaceBetween);

    for (int i = 0; i < 3; ++i) {
        auto choice = make_item("choice_" + std::to_string(i));
        choice->set_flex_grow(1.0f);
        choices_row->add_child(choice);
    }
    root->add_child(choices_row);

    root->layout(0, 0, m_screen_width, m_screen_height);

    // Gán kết quả tính toán vào các vùng Rect
    m_image_rect = Rect{ hero->computed_x, hero->computed_y, hero->computed_w, hero->computed_h };
    m_text_rect = Rect{ text_card->computed_x, text_card->computed_y, text_card->computed_w, text_card->computed_h };
    m_phonics_word_rect = Rect{ phonics_tray->computed_x, phonics_tray->computed_y, phonics_tray->computed_w, phonics_tray->computed_h };

    for (int i = 0; i < 3; ++i) {
        auto c = choices_row->children[i];
        m_choice_rects[i] = Rect{ c->computed_x, c->computed_y, c->computed_w, c->computed_h };
    }
    m_choice_rects[3] = Rect{ 0, 0, 0, 0 };

    // Nút chuyển trang (Next CTA) căn giữa
    int next_w = static_cast<int>(480 * scale);
    int next_h = static_cast<int>(100 * scale);
    int next_x = (m_screen_width - next_w) / 2;
    int next_y = choices_row->computed_y + (choices_row->computed_h - next_h) / 2;
    m_next_btn_rect = Rect{ next_x, next_y, next_w, next_h };

    // 3. TÍNH TOÁN BỐ CỤC MỤC LỤC OVERLAY (TOC MODAL)
    int toc_w = static_cast<int>(940 * scale);
    int toc_h = static_cast<int>(1140 * scale);
    int toc_x = (m_screen_width - toc_w) / 2;
    int toc_y = (m_screen_height - toc_h) / 2;
    m_toc_card_rect = Rect{ toc_x, toc_y, toc_w, toc_h };

    m_toc_close_btn_rect = Rect{
        toc_x + toc_w - static_cast<int>(110 * scale),
        toc_y + static_cast<int>(36 * scale),
        static_cast<int>(75 * scale),
        static_cast<int>(36 * scale)
    };

    int grid_start_x = toc_x + static_cast<int>(48 * scale);
    int grid_start_y = toc_y + static_cast<int>(115 * scale);
    int item_gap_x = static_cast<int>(22 * scale);
    int item_gap_y = static_cast<int>(22 * scale);
    int item_w = (toc_w - static_cast<int>(96 * scale) - 3 * item_gap_x) / 4;
    int item_h = static_cast<int>(220 * scale);

    for (int i = 0; i < 16; ++i) {
        int row = i / 4;
        int col = i % 4;
        int ix = grid_start_x + col * (item_w + item_gap_x);
        int iy = grid_start_y + row * (item_h + item_gap_y);
        m_toc_item_rects[i] = Rect{ ix, iy, item_w, item_h };
    }

    // 4. TÍNH TOÁN BỐ CỤC CÀI ĐẶT OVERLAY (SETTINGS MODAL)
    int stg_w = static_cast<int>(780 * scale);
    int stg_h = static_cast<int>(600 * scale);
    int stg_x = (m_screen_width - stg_w) / 2;
    int stg_y = (m_screen_height - stg_h) / 2;
    m_settings_card_rect = Rect{ stg_x, stg_y, stg_w, stg_h };

    m_settings_close_btn_rect = Rect{
        stg_x + stg_w - static_cast<int>(110 * scale),
        stg_y + static_cast<int>(36 * scale),
        static_cast<int>(75 * scale),
        static_cast<int>(36 * scale)
    };

    int item1_y = stg_y + static_cast<int>(130 * scale);
    int tog_w = static_cast<int>(110 * scale);
    int tog_h = static_cast<int>(50 * scale);
    int tog_x = stg_x + stg_w - static_cast<int>(60 * scale) - tog_w;
    int tog_y = item1_y + static_cast<int>(20 * scale);
    m_settings_toggle_hint_rect = Rect{ tog_x, tog_y, tog_w, tog_h };

    int item2_y = item1_y + static_cast<int>(110 * scale);
    int rst_w = static_cast<int>(170 * scale);
    int rst_h = static_cast<int>(50 * scale);
    int rst_x = stg_x + stg_w - static_cast<int>(60 * scale) - rst_w;
    int rst_y = item2_y + static_cast<int>(20 * scale);
    m_settings_reset_btn_rect = Rect{ rst_x, rst_y, rst_w, rst_h };

    int done_w = static_cast<int>(320 * scale);
    int done_h = static_cast<int>(75 * scale);
    int done_x = stg_x + (stg_w - done_w) / 2;
    int done_y = stg_y + stg_h - static_cast<int>(110 * scale);
    m_settings_done_btn_rect = Rect{ done_x, done_y, done_w, done_h };
}

bool UILayout::hit_test_back(int px, int py) const noexcept {
    return m_back_btn_rect.contains(px, py);
}

bool UILayout::hit_test_menu(int px, int py) const noexcept {
    return m_menu_btn_rect.contains(px, py);
}

bool UILayout::hit_test_settings(int px, int py) const noexcept {
    return m_settings_btn_rect.contains(px, py);
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

bool UILayout::hit_test_toc_close(int px, int py) const noexcept {
    return m_toc_close_btn_rect.contains(px, py) || !m_toc_card_rect.contains(px, py);
}

int UILayout::hit_test_toc_item(int px, int py) const noexcept {
    for (int i = 0; i < 16; ++i) {
        if (m_toc_item_rects[i].contains(px, py)) {
            return i + 1; // Trả về trang 1..16
        }
    }
    return -1;
}

bool UILayout::hit_test_settings_close(int px, int py) const noexcept {
    return m_settings_close_btn_rect.contains(px, py) || m_settings_done_btn_rect.contains(px, py) || !m_settings_card_rect.contains(px, py);
}

bool UILayout::hit_test_settings_toggle_hint(int px, int py) const noexcept {
    return m_settings_toggle_hint_rect.contains(px, py);
}

bool UILayout::hit_test_settings_reset(int px, int py) const noexcept {
    return m_settings_reset_btn_rect.contains(px, py);
}

} // namespace gach::adapters::ui
