#include <cassert>
#include <chrono>
#include <iostream>
#include "adapters/ui/ink_flex.hpp"

using namespace gach::adapters::ui;

void test_column_layout_non_overlapping() {
    auto root = make_column("root");
    root->set_size(1000, 2000);
    root->set_padding(Edges::all(20));
    root->set_gap(15);

    auto header = make_item("header");
    header->set_height(80);

    auto text = make_item("text");
    text->set_height(120);

    auto footer = make_item("footer");
    footer->set_height(100);

    root->add_child(header);
    root->add_child(text);
    root->add_child(footer);

    root->layout(0, 0, 1000, 2000);

    // Kiểm tra không chồng lấn toán học
    assert(header->computed_y == 20);
    assert(header->computed_h == 80);

    assert(text->computed_y == 20 + 80 + 15); // 115
    assert(text->computed_h == 120);

    assert(footer->computed_y == 115 + 120 + 15); // 250
    assert(footer->computed_h == 100);

    std::cout << "  ✓ test_column_layout_non_overlapping passed" << std::endl;
}

void test_flex_grow_dynamic_expansion() {
    auto root = make_column("root");
    root->set_size(1000, 1000);
    root->set_padding(Edges::all(0));
    root->set_gap(0);

    auto fixed_box = make_item("fixed");
    fixed_box->set_height(200);

    auto expanding_box = make_item("expanding");
    expanding_box->set_flex_grow(1.0f); // Tự bung chiếm trọn 800px còn lại!

    root->add_child(fixed_box);
    root->add_child(expanding_box);

    root->layout(0, 0, 1000, 1000);

    assert(fixed_box->computed_h == 200);
    assert(expanding_box->computed_y == 200);
    assert(expanding_box->computed_h == 800); // 1000 - 200 = 800!

    std::cout << "  ✓ test_flex_grow_dynamic_expansion passed (expanded to 800px)" << std::endl;
}

void test_absolute_positioning_and_z_index() {
    auto root = make_item("root");
    root->set_size(1000, 1000);

    auto background = make_item("bg");
    background->set_size(1000, 1000);
    background->set_z_index(0);

    auto floating_btn = make_item("btn_exit");
    floating_btn->set_position(PositionType::Absolute);
    floating_btn->set_top(15);
    floating_btn->set_right(20);
    floating_btn->set_size(120, 50);
    floating_btn->set_z_index(10); // Nằm đè lên trên

    root->add_child(background);
    root->add_child(floating_btn);

    root->layout(0, 0, 1000, 1000);

    assert(floating_btn->computed_x == 1000 - 20 - 120); // 860
    assert(floating_btn->computed_y == 15);
    assert(floating_btn->computed_w == 120);
    assert(floating_btn->computed_h == 50);

    // Hit testing: Click tại (870, 25) trúng floating_btn vì z-index = 10 cao hơn background (z-index = 0)
    FlexNode* hit = root->hit_test(870, 25);
    assert(hit != nullptr);
    assert(hit->id == "btn_exit");

    // Click tại (100, 100) trúng background
    FlexNode* hit_bg = root->hit_test(100, 100);
    assert(hit_bg != nullptr);
    assert(hit_bg->id == "bg");

    std::cout << "  ✓ test_absolute_positioning_and_z_index passed" << std::endl;
}

void test_benchmark_performance() {
    auto root = make_column("root");
    root->set_size(1072, 1448);
    root->set_padding(Edges::all(32));
    root->set_gap(20);

    auto img = make_item("img");
    img->set_flex_grow(1.0f);

    auto txt = make_item("txt");
    txt->set_height(160);

    auto tray = make_item("tray");
    tray->set_height(100);

    auto row = make_row("choices");
    row->set_height(140);
    row->set_gap(20);
    for (int i = 0; i < 3; ++i) {
        auto c = make_item("c" + std::to_string(i));
        c->set_flex_grow(1.0f);
        row->add_child(c);
    }

    root->add_child(img);
    root->add_child(txt);
    root->add_child(tray);
    root->add_child(row);

    const int iterations = 10000;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        root->layout(0, 0, 1072, 1448);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::micro> elapsed = end - start;
    double avg_us = elapsed.count() / iterations;

    std::cout << "  ✓ 10,000 Flexbox layouts calculated in " << elapsed.count() / 1000.0 << " ms (Avg: " << avg_us << " µs/layout)" << std::endl;
    assert(avg_us < 20.0 && "Layout must take < 20 microseconds");
}

int main() {
    std::cout << "=== Running InkFlex Layout Engine Unit Tests ===" << std::endl;
    test_column_layout_non_overlapping();
    test_flex_grow_dynamic_expansion();
    test_absolute_positioning_and_z_index();
    test_benchmark_performance();
    std::cout << "All InkFlex tests passed!" << std::endl;
    return 0;
}
