#include <cairo.h>
#include <iostream>
#include <string>
#include <cmath>

void draw_scene(const std::string& filename, const std::string& title, const std::string& sub, bool is_bloom) {
    const int width = 1072;
    const int height = 1014;

    cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cairo_t* cr = cairo_create(surface);

    // 1. Nền trắng E-Ink
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_paint(cr);

    // 2. Viền khung tranh E-Ink
    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
    cairo_set_line_width(cr, 6.0);
    cairo_rectangle(cr, 20, 20, width - 40, height - 40);
    cairo_stroke(cr);

    // 3. Đường chân trời / mặt đất
    cairo_set_line_width(cr, 4.0);
    cairo_move_to(cr, 40, height * 0.75);
    cairo_line_to(cr, width - 40, height * 0.75);
    cairo_stroke(cr);

    // 4. Vẽ bạn Mít (Nét mực E-Ink giản dị, ấm áp)
    double mit_x = width * 0.35;
    double mit_y = height * 0.62;

    // Đầu Mít
    cairo_set_line_width(cr, 5.0);
    cairo_arc(cr, mit_x, mit_y - 120, 50, 0, 2 * M_PI);
    cairo_stroke(cr);

    // Tóc Mít (3 sợi tóc chỏm dễ thương)
    cairo_move_to(cr, mit_x - 10, mit_y - 170);
    cairo_line_to(cr, mit_x - 15, mit_y - 195);
    cairo_move_to(cr, mit_x, mit_y - 170);
    cairo_line_to(cr, mit_x, mit_y - 200);
    cairo_move_to(cr, mit_x + 10, mit_y - 170);
    cairo_line_to(cr, mit_x + 15, mit_y - 195);
    cairo_stroke(cr);

    // Mắt & Miệng Mít mỉm cười
    cairo_arc(cr, mit_x - 18, mit_y - 125, 4, 0, 2 * M_PI);
    cairo_arc(cr, mit_x + 18, mit_y - 125, 4, 0, 2 * M_PI);
    cairo_fill(cr);

    cairo_arc(cr, mit_x, mit_y - 110, 20, 0.2 * M_PI, 0.8 * M_PI);
    cairo_stroke(cr);

    // Thân áo Mít
    cairo_move_to(cr, mit_x - 40, mit_y - 70);
    cairo_line_to(cr, mit_x + 40, mit_y - 70);
    cairo_line_to(cr, mit_x + 50, mit_y + 60);
    cairo_line_to(cr, mit_x - 50, mit_y + 60);
    cairo_close_path(cr);
    cairo_stroke(cr);

    // 5. Khóm cây / hoa
    double plant_x = width * 0.70;
    double plant_y = height * 0.75;

    // Chậu cây
    cairo_move_to(cr, plant_x - 60, plant_y);
    cairo_line_to(cr, plant_x + 60, plant_y);
    cairo_line_to(cr, plant_x + 45, plant_y + 60);
    cairo_line_to(cr, plant_x - 45, plant_y + 60);
    cairo_close_path(cr);
    cairo_stroke(cr);

    // Thân cây
    cairo_move_to(cr, plant_x, plant_y);
    cairo_curve_to(cr, plant_x - 10, plant_y - 100, plant_x + 10, plant_y - 150, plant_x, plant_y - 220);
    cairo_stroke(cr);

    if (is_bloom) {
        // Cảnh quả ngọt: Hoa nở rộ to tròn, lá sum suê
        for (int i = 0; i < 6; ++i) {
            double angle = i * (M_PI / 3.0);
            double petal_x = plant_x + 45 * std::cos(angle);
            double petal_y = (plant_y - 220) + 45 * std::sin(angle);
            cairo_arc(cr, petal_x, petal_y, 25, 0, 2 * M_PI);
            cairo_stroke(cr);
        }
        // Nhụy hoa
        cairo_arc(cr, plant_x, plant_y - 220, 20, 0, 2 * M_PI);
        cairo_fill(cr);

        // Mặt trời mỉm cười chiếu sáng
        cairo_arc(cr, 150, 150, 60, 0, 2 * M_PI);
        cairo_stroke(cr);
        for (int i = 0; i < 8; ++i) {
            double a = i * (M_PI / 4.0);
            cairo_move_to(cr, 150 + 75 * std::cos(a), 150 + 75 * std::sin(a));
            cairo_line_to(cr, 150 + 100 * std::cos(a), 150 + 100 * std::sin(a));
            cairo_stroke(cr);
        }
    } else {
        // Cảnh bắt đầu: Cành lá rũ xuống khát nước
        cairo_arc(cr, plant_x - 30, plant_y - 160, 15, 0, M_PI);
        cairo_stroke(cr);
        cairo_arc(cr, plant_x + 30, plant_y - 120, 15, 0, M_PI);
        cairo_stroke(cr);
    }

    // 6. Tiêu đề cảnh
    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
    cairo_select_font_face(cr, "serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 38.0);
    cairo_move_to(cr, 60, 90);
    cairo_show_text(cr, title.c_str());

    cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_ITALIC, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 26.0);
    cairo_move_to(cr, 60, 135);
    cairo_show_text(cr, sub.c_str());

    cairo_destroy(cr);
    cairo_surface_write_to_png(surface, filename.c_str());
    cairo_surface_destroy(surface);
    std::cout << "Generated: " << filename << std::endl;
}

int main() {
    draw_scene("assets/images/page1_dry.png", "Mít tưới cây cho bà (Trang 1)", "Cây đang khát nước giữa trưa hè nắng gắt", false);
    draw_scene("assets/images/page1_bloom.png", "Cây được uống nước mát lành", "Khóm hoa tươi cười, nở rộ rực rỡ", true);

    draw_scene("assets/images/page2_leaves.png", "Dòng nước tưới mát (Trang 2)", "Từng giọt nước thấm sâu vào đất ấm", false);
    draw_scene("assets/images/page2_green.png", "Lá xanh mướt rung rinh", "Khu vườn bừng sáng sức sống an vui", true);

    draw_scene("assets/images/page3_grandma.png", "Bà bước ra hiên (Trang 3)", "Bà nhìn khu vườn xanh mướt mỉm cười", false);
    draw_scene("assets/images/page3_hug.png", "Bà xoa đầu khen Mít", "Trái tim chan chứa yêu thương và lòng tử tế", true);

    return 0;
}
