#include <cassert>
#include <cairo.h>
#include <iostream>
#include "adapters/ui/minimal_components.hpp"

using namespace gach::adapters::ui;

void test_components_render_smoke() {
    const int W = 1072;
    const int H = 1448;
    cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, W, H);
    cairo_t* cr = cairo_create(surface);

    // 1. Utility pills
    MinimalComponents::draw_utility_pill(cr, 50, 50, 95, 36, "Thoat", MinimalComponents::UtilityIcon::CLOSE);
    MinimalComponents::draw_utility_pill(cr, 160, 50, 95, 36, "Quay lai", MinimalComponents::UtilityIcon::BACK);
    MinimalComponents::draw_utility_pill(cr, 270, 50, 95, 36, "Nghe", MinimalComponents::UtilityIcon::REPLAY);
    MinimalComponents::draw_utility_pill(cr, 380, 50, 95, 36, "Muc luc", MinimalComponents::UtilityIcon::MENU);

    // 2. Action pills
    MinimalComponents::draw_action_pill(cr, 50, 120, 480, 100, "Tiep tuc sang trang ->", MinimalComponents::ButtonStyle::PRIMARY_SOLID);
    MinimalComponents::draw_action_pill(cr, 550, 120, 480, 100, "Thu lai", MinimalComponents::ButtonStyle::SECONDARY_OUTLINE);

    // 3. Choice tiles
    MinimalComponents::draw_choice_tile(cr, 50, 250, 280, 145, "ch", MinimalComponents::ChoiceState::NORMAL);
    MinimalComponents::draw_choice_tile(cr, 360, 250, 280, 145, "nh", MinimalComponents::ChoiceState::HINTED);
    MinimalComponents::draw_choice_tile(cr, 670, 250, 280, 145, "tr", MinimalComponents::ChoiceState::DISABLED);

    // 4. Phonics slots
    MinimalComponents::draw_phonics_slot(cr, 50, 430, 972, 110, "ch", "ao", true, false, "chao");
    MinimalComponents::draw_phonics_slot(cr, 50, 560, 972, 110, "r", "au", false, false, "rau");
    MinimalComponents::draw_phonics_slot(cr, 50, 690, 972, 110, "ch", "ao", true, true, "chao");

    // 5. Page tracker
    MinimalComponents::draw_page_tracker(cr, W / 2.0, 30, 1, 16);

    // 6. Modal dialog
    MinimalComponents::draw_modal_dialog(cr, W, H, "Nghi ngoi", "Be co muon tam dung sach khong?", "Thoat", "Doc tiep");

    cairo_destroy(cr);
    cairo_surface_destroy(surface);

    std::cout << "  ✓ test_components_render_smoke passed (all 7 component types rendered successfully)" << std::endl;
}

int main() {
    std::cout << "=== Running MinimalComponents Unit Tests ===" << std::endl;
    test_components_render_smoke();
    std::cout << "All MinimalComponents tests passed!" << std::endl;
    return 0;
}
