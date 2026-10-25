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

void draw_ehon_illustration(const std::string& filename, int page, bool is_bloom) {
    const int W = 1072;
    const int H = 720;

    cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, W, H);
    cairo_t* cr = cairo_create(surface);

    // 1. Nền giấy E-Ink trắng tinh
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_paint(cr);

    // 2. Nét mực đen E-Ink sắc nét
    cairo_set_source_rgb(cr, 0.12, 0.12, 0.12);

    // 3. Đường mặt đất / sàn nhà (Tối giản không viền khung đóng hộp)
    cairo_set_line_width(cr, 3.5);
    cairo_move_to(cr, 30, H * 0.72);
    cairo_curve_to(cr, W * 0.3, H * 0.70, W * 0.6, H * 0.74, W - 30, H * 0.72);
    cairo_stroke(cr);

    // Bụi cỏ / họa tiết sàn nhà
    for (int gx : {90, 180, 880, 970}) {
        cairo_move_to(cr, gx, H * 0.72);
        cairo_line_to(cr, gx - 10, H * 0.68);
        cairo_move_to(cr, gx, H * 0.72);
        cairo_line_to(cr, gx + 10, H * 0.67);
        cairo_stroke(cr);
    }

    // 4. Nhân vật chính: BẠN MÍT
    double mx = W * 0.35;
    double my = H * 0.58;

    cairo_set_line_width(cr, 4.5);
    // Đầu Mít tròn
    cairo_arc(cr, mx, my - 130, 60, 0, 2 * M_PI);
    cairo_stroke(cr);

    // 3 sợi tóc chỏm
    cairo_move_to(cr, mx - 12, my - 190);
    cairo_line_to(cr, mx - 18, my - 225);
    cairo_move_to(cr, mx, my - 190);
    cairo_line_to(cr, mx, my - 230);
    cairo_move_to(cr, mx + 12, my - 190);
    cairo_line_to(cr, mx + 18, my - 225);
    cairo_stroke(cr);

    // Mắt cười & Miệng cười
    cairo_arc(cr, mx - 22, my - 135, 6, 0, 2 * M_PI);
    cairo_arc(cr, mx + 22, my - 135, 6, 0, 2 * M_PI);
    cairo_fill(cr);

    cairo_arc(cr, mx, my - 115, 24, 0.2 * M_PI, 0.8 * M_PI);
    cairo_stroke(cr);

    // Má hồng
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

    // 5. Nhân vật phụ / Vật phẩm theo từng trang:
    double ox = W * 0.72;
    double oy = H * 0.65;

    switch (page) {
        case 1: // Chào ông bà
        case 10: // Hái lá biếu bà
        case 14: // Dâng nước mời ông
        {
            // Nhân vật Ông / Bà hiền hậu
            cairo_arc(cr, ox, oy - 120, 55, 0, 2 * M_PI);
            cairo_stroke(cr);
            // Búi tóc bà hoặc nón ông
            cairo_arc(cr, ox - 50, oy - 130, 22, 0, 2 * M_PI);
            cairo_stroke(cr);
            // Mắt cười phúc hậu
            cairo_arc(cr, ox - 16, oy - 125, 12, 0.1 * M_PI, 0.9 * M_PI);
            cairo_arc(cr, ox + 16, oy - 125, 12, 0.1 * M_PI, 0.9 * M_PI);
            cairo_stroke(cr);
            // Thân áo
            cairo_move_to(cr, ox - 50, oy - 65);
            cairo_line_to(cr, ox + 50, oy - 65);
            cairo_line_to(cr, ox + 65, oy + 75);
            cairo_line_to(cr, ox - 65, oy + 75);
            cairo_close_path(cr);
            cairo_stroke(cr);
            break;
        }

        case 3: // Cho mèo ăn cơm
        {
            // Chú mèo con
            cairo_arc(cr, ox, oy + 20, 35, 0, 2 * M_PI);
            cairo_stroke(cr);
            // Tai mèo
            cairo_move_to(cr, ox - 25, oy - 5);
            cairo_line_to(cr, ox - 35, oy - 35);
            cairo_line_to(cr, ox - 10, oy - 15);
            cairo_stroke(cr);
            cairo_move_to(cr, ox + 25, oy - 5);
            cairo_line_to(cr, ox + 35, oy - 35);
            cairo_line_to(cr, ox + 10, oy - 15);
            cairo_stroke(cr);
            // Mắt mèo
            cairo_arc(cr, ox - 12, oy + 15, 4, 0, 2 * M_PI);
            cairo_arc(cr, ox + 12, oy + 15, 4, 0, 2 * M_PI);
            cairo_fill(cr);
            // Bát cơm
            cairo_arc(cr, ox - 50, oy + 45, 20, 0, M_PI);
            cairo_stroke(cr);
            break;
        }

        case 5: // Tưới hoa
        case 13: // Chăm sóc búp mầm
        default:
        {
            // Chậu hoa / Cây cảnh
            cairo_move_to(cr, ox - 60, oy + 40);
            cairo_line_to(cr, ox + 60, oy + 40);
            cairo_line_to(cr, ox + 45, oy + 95);
            cairo_line_to(cr, ox - 45, oy + 95);
            cairo_close_path(cr);
            cairo_stroke(cr);

            // Thân cây
            cairo_move_to(cr, ox, oy + 40);
            cairo_curve_to(cr, ox - 15, oy - 60, ox + 15, oy - 120, ox, oy - 180);
            cairo_stroke(cr);

            if (is_bloom) {
                // Bông hoa nở to rực rỡ
                for (int i = 0; i < 6; ++i) {
                    double a = i * (M_PI / 3.0);
                    cairo_arc(cr, ox + 45 * std::cos(a), (oy - 180) + 45 * std::sin(a), 24, 0, 2 * M_PI);
                    cairo_stroke(cr);
                }
                cairo_arc(cr, ox, oy - 180, 20, 0, 2 * M_PI);
                cairo_fill(cr);
            } else {
                // Búp nụ hoặc cành khô
                cairo_arc(cr, ox, oy - 180, 18, 0, M_PI);
                cairo_stroke(cr);
            }
            break;
        }
    }

    if (is_bloom) {
        // Mặt trời mỉm cười ấm áp góc trời
        cairo_arc(cr, 110, 100, 40, 0, 2 * M_PI);
        cairo_stroke(cr);
        for (int i = 0; i < 8; ++i) {
            double a = i * (M_PI / 4.0);
            cairo_move_to(cr, 110 + 48 * std::cos(a), 100 + 48 * std::sin(a));
            cairo_line_to(cr, 110 + 65 * std::cos(a), 100 + 65 * std::sin(a));
            cairo_stroke(cr);
        }

        // Trái tim yêu thương nhỏ bay lên ở giữa
        double hx = W * 0.52;
        double hy = H * 0.28;
        cairo_move_to(cr, hx, hy);
        cairo_curve_to(cr, hx - 25, hy - 40, hx - 45, hy - 10, hx, hy + 35);
        cairo_curve_to(cr, hx + 45, hy - 10, hx + 25, hy - 40, hx, hy);
        cairo_fill(cr);
    }

    cairo_destroy(cr);
    cairo_surface_write_to_png(surface, filename.c_str());
    cairo_surface_destroy(surface);
}

int main() {
    std::cout << "Generating 32 Ehon illustration assets for 16-page story..." << std::endl;
    for (int p = 1; p <= 16; ++p) {
        std::string dry_name = "assets/images/page" + std::to_string(p) + "_dry.png";
        std::string bloom_name = "assets/images/page" + std::to_string(p) + "_bloom.png";

        draw_ehon_illustration(dry_name, p, false);
        draw_ehon_illustration(bloom_name, p, true);
    }
    std::cout << "All 32 illustrations generated successfully!" << std::endl;
    return 0;
}
