#include <cairo.h>
#include <iostream>
#include <string>
#include <cmath>

void draw_rounded_rect(cairo_t* cr, double x, double y, double w, double h, double r) {
    cairo_new_sub_path(cr);
    cairo_arc(cr, x + w - r, y + r, r, -M_PI/2, 0);
    cairo_arc(cr, x + w - r, y + h - r, r, 0, M_PI/2);
    cairo_arc(cr, x + r, y + h - r, r, M_PI/2, M_PI);
    cairo_arc(cr, x + r, y + r, r, M_PI, 3*M_PI/2);
    cairo_close_path(cr);
}

void draw_scene_art(const std::string& filename, int page, bool is_bloom) {
    const int W = 1072;
    const int H = 720; // Khung tranh 1072x720 vừa khít tỷ lệ vàng E-Ink

    cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, W, H);
    cairo_t* cr = cairo_create(surface);

    // 1. Nền trắng tinh khiết
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_paint(cr);

    // 2. Viền khung tranh bo góc tinh tế
    cairo_set_source_rgb(cr, 0.15, 0.15, 0.15);
    cairo_set_line_width(cr, 4.0);
    draw_rounded_rect(cr, 16, 16, W - 32, H - 32, 20.0);
    cairo_stroke(cr);

    // 3. Đường mặt đất / thảm cỏ mềm mại
    cairo_set_line_width(cr, 3.5);
    cairo_move_to(cr, 30, H * 0.72);
    cairo_curve_to(cr, W * 0.3, H * 0.70, W * 0.6, H * 0.74, W - 30, H * 0.72);
    cairo_stroke(cr);

    // Vẽ vài cụm cỏ nhỏ ở góc
    for (int gx : {80, 140, 220, 880, 960}) {
        cairo_move_to(cr, gx, H * 0.72);
        cairo_line_to(cr, gx - 12, H * 0.68);
        cairo_move_to(cr, gx, H * 0.72);
        cairo_line_to(cr, gx + 10, H * 0.67);
        cairo_stroke(cr);
    }

    if (page == 1 || page == 2) {
        // --- VẼ BẠN MÍT VÀ BÌNH TƯỚI NƯỚC ---
        double mx = W * 0.32;
        double my = H * 0.58;

        // Đầu bạn Mít (tròn trĩnh, to rõ)
        cairo_set_line_width(cr, 4.5);
        cairo_arc(cr, mx, my - 130, 60, 0, 2 * M_PI);
        cairo_stroke(cr);

        // Chỏm tóc đáng yêu
        cairo_move_to(cr, mx - 12, my - 190);
        cairo_line_to(cr, mx - 18, my - 225);
        cairo_move_to(cr, mx, my - 190);
        cairo_line_to(cr, mx, my - 230);
        cairo_move_to(cr, mx + 12, my - 190);
        cairo_line_to(cr, mx + 18, my - 225);
        cairo_stroke(cr);

        // Mắt tròn lấp lánh & miệng cười
        cairo_arc(cr, mx - 22, my - 135, 6, 0, 2 * M_PI);
        cairo_arc(cr, mx + 22, my - 135, 6, 0, 2 * M_PI);
        cairo_fill(cr);

        cairo_arc(cr, mx, my - 115, 24, 0.2 * M_PI, 0.8 * M_PI);
        cairo_stroke(cr);

        // Má hồng chấm bi
        cairo_arc(cr, mx - 38, my - 120, 3, 0, 2 * M_PI);
        cairo_arc(cr, mx + 38, my - 120, 3, 0, 2 * M_PI);
        cairo_fill(cr);

        // Áo bạn Mít
        cairo_move_to(cr, mx - 45, my - 70);
        cairo_line_to(cr, mx + 45, my - 70);
        cairo_line_to(cr, mx + 60, my + 80);
        cairo_line_to(cr, mx - 60, my + 80);
        cairo_close_path(cr);
        cairo_stroke(cr);

        // Tay Mít cầm bình tưới nước
        cairo_move_to(cr, mx + 40, my - 30);
        cairo_line_to(cr, mx + 100, my + 10);
        cairo_stroke(cr);

        // Bình tưới nước nhỏ xinh
        double can_x = mx + 130;
        double can_y = my + 20;
        draw_rounded_rect(cr, can_x - 30, can_y - 25, 60, 50, 8);
        cairo_stroke(cr);
        // Quai bình
        cairo_arc(cr, can_x - 25, can_y, 18, -M_PI/2, M_PI/2);
        cairo_stroke(cr);
        // Vòi tưới nước nghiêng xuống khóm hoa
        cairo_move_to(cr, can_x + 30, can_y - 10);
        cairo_line_to(cr, can_x + 80, can_y + 35);
        cairo_stroke(cr);

        // --- KHÓM HOA CỦA BÀ ---
        double px = W * 0.74;
        double py = H * 0.72;

        // Chậu hoa gốm mộc mạc
        cairo_move_to(cr, px - 75, py);
        cairo_line_to(cr, px + 75, py);
        cairo_line_to(cr, px + 55, py + 75);
        cairo_line_to(cr, px - 55, py + 75);
        cairo_close_path(cr);
        cairo_stroke(cr);
        // Vân viền chậu
        cairo_move_to(cr, px - 70, py + 20);
        cairo_line_to(cr, px + 70, py + 20);
        cairo_stroke(cr);

        // Thân cành hoa
        cairo_move_to(cr, px, py);
        cairo_curve_to(cr, px - 15, py - 90, px + 15, py - 160, px, py - 230);
        cairo_stroke(cr);

        if (is_bloom) {
            // Giọt nước lấp lánh từ vòi tưới
            for (int i = 0; i < 4; ++i) {
                cairo_arc(cr, can_x + 85 + i * 25, can_y + 45 + i * 20, 5, 0, 2 * M_PI);
                cairo_fill(cr);
            }

            // Hoa nở bừng rực rỡ (6 cánh hoa to tròn)
            for (int i = 0; i < 6; ++i) {
                double a = i * (M_PI / 3.0);
                double fx = px + 55 * std::cos(a);
                double fy = (py - 230) + 55 * std::sin(a);
                cairo_arc(cr, fx, fy, 28, 0, 2 * M_PI);
                cairo_stroke(cr);
            }
            cairo_arc(cr, px, py - 230, 24, 0, 2 * M_PI);
            cairo_fill(cr);

            // Lá xanh xòe rộng
            cairo_arc(cr, px - 45, py - 140, 30, 0, M_PI);
            cairo_stroke(cr);
            cairo_arc(cr, px + 45, py - 100, 30, 0, M_PI);
            cairo_stroke(cr);

            // Mặt trời mùa hè ấm áp ở góc trời
            cairo_arc(cr, 120, 110, 45, 0, 2 * M_PI);
            cairo_stroke(cr);
            for (int i = 0; i < 8; ++i) {
                double a = i * (M_PI / 4.0);
                cairo_move_to(cr, 120 + 55 * std::cos(a), 110 + 55 * std::sin(a));
                cairo_line_to(cr, 120 + 75 * std::cos(a), 110 + 75 * std::sin(a));
                cairo_stroke(cr);
            }
        } else {
            // Cành lá rũ xuống héo khô khát nước
            cairo_move_to(cr, px, py - 140);
            cairo_curve_to(cr, px - 40, py - 130, px - 50, py - 90, px - 45, py - 70);
            cairo_stroke(cr);

            cairo_move_to(cr, px, py - 180);
            cairo_curve_to(cr, px + 40, py - 170, px + 50, py - 130, px + 45, py - 110);
            cairo_stroke(cr);

            // Bông nụ gục đầu
            cairo_arc(cr, px, py - 220, 20, 0, M_PI);
            cairo_stroke(cr);
        }
    } else {
        // --- TRANG 3: BÀ VÀ BẠN MÍT ---
        // Bà đứng hiền từ bên cạnh Mít
        double bx = W * 0.40;
        double by = H * 0.52;
        // Đầu bà với búi tóc
        cairo_set_line_width(cr, 4.5);
        cairo_arc(cr, bx, by - 120, 55, 0, 2 * M_PI);
        cairo_stroke(cr);
        // Búi tóc bà
        cairo_arc(cr, bx - 55, by - 130, 25, 0, 2 * M_PI);
        cairo_stroke(cr);
        // Mắt cười phúc hậu
        cairo_arc(cr, bx - 15, by - 125, 12, 0.1 * M_PI, 0.9 * M_PI);
        cairo_stroke(cr);
        cairo_arc(cr, bx + 15, by - 125, 12, 0.1 * M_PI, 0.9 * M_PI);
        cairo_stroke(cr);
        cairo_arc(cr, bx, by - 100, 18, 0.2 * M_PI, 0.8 * M_PI);
        cairo_stroke(cr);

        // Áo bà
        cairo_move_to(cr, bx - 50, by - 65);
        cairo_line_to(cr, bx + 50, by - 65);
        cairo_line_to(cr, bx + 70, by + 120);
        cairo_line_to(cr, bx - 70, by + 120);
        cairo_close_path(cr);
        cairo_stroke(cr);

        // Bạn Mít bên cạnh bà
        double mx = W * 0.65;
        double my = H * 0.62;
        cairo_arc(cr, mx, my - 100, 48, 0, 2 * M_PI);
        cairo_stroke(cr);
        cairo_arc(cr, mx - 16, my - 105, 5, 0, 2 * M_PI);
        cairo_arc(cr, mx + 16, my - 105, 5, 0, 2 * M_PI);
        cairo_fill(cr);
        cairo_arc(cr, mx, my - 88, 18, 0.2 * M_PI, 0.8 * M_PI);
        cairo_stroke(cr);

        cairo_move_to(cr, mx - 38, my - 52);
        cairo_line_to(cr, mx + 38, my - 52);
        cairo_line_to(cr, mx + 50, my + 70);
        cairo_line_to(cr, mx - 50, my + 70);
        cairo_close_path(cr);
        cairo_stroke(cr);

        if (is_bloom) {
            // Tay bà xoa đầu Mít yêu thương
            cairo_move_to(cr, bx + 45, by - 20);
            cairo_curve_to(cr, bx + 80, by - 70, mx - 30, my - 160, mx, my - 148);
            cairo_stroke(cr);

            // Trái tim yêu thương nhỏ bay lên
            cairo_move_to(cr, W * 0.52, H * 0.28);
            cairo_curve_to(cr, W * 0.50, H * 0.23, W * 0.46, H * 0.25, W * 0.52, H * 0.33);
            cairo_curve_to(cr, W * 0.58, H * 0.25, W * 0.54, H * 0.23, W * 0.52, H * 0.28);
            cairo_fill(cr);
        }
    }

    cairo_destroy(cr);
    cairo_surface_write_to_png(surface, filename.c_str());
    cairo_surface_destroy(surface);
    std::cout << "Rendered artwork: " << filename << std::endl;
}

int main() {
    draw_scene_art("assets/images/page1_dry.png", 1, false);
    draw_scene_art("assets/images/page1_bloom.png", 1, true);

    draw_scene_art("assets/images/page2_leaves.png", 2, false);
    draw_scene_art("assets/images/page2_green.png", 2, true);

    draw_scene_art("assets/images/page3_grandma.png", 3, false);
    draw_scene_art("assets/images/page3_hug.png", 3, true);

    return 0;
}
