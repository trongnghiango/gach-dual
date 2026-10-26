#include <cairo.h>
#include <iostream>
#include <string>
#include <cmath>

// ==============================================================================
// GACH STORY PHONICS ENGINE - ARTWORK GENERATOR (OPAQUE SOLID EDITION)
// Minimalist Ehon Picture Book Style (E-Ink Carta 300 PPI)
// Solid White Fills + Crisp 4.5px Black Contours (Zero Visual Clutter)
// 16 Dedicated Contextual Scenes for Comprehensible Input (CI)
// ==============================================================================

const double W = 1072.0;
const double H = 720.0;
const double GY = 560.0; // Standard ground line

void stroke_shape(cairo_t* cr, bool fill_white = true) {
    if (fill_white) {
        cairo_save(cr);
        cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
        cairo_fill_preserve(cr);
        cairo_restore(cr);
    }
    cairo_set_source_rgb(cr, 0.08, 0.08, 0.08);
    cairo_stroke(cr);
}

void draw_rounded_rect(cairo_t* cr, double x, double y, double w, double h, double r, bool fill_white = true) {
    cairo_new_sub_path(cr);
    cairo_arc(cr, x + w - r, y + r, r, -M_PI/2, 0);
    cairo_arc(cr, x + w - r, y + h - r, r, 0, M_PI/2);
    cairo_arc(cr, x + r, y + h - r, r, M_PI/2, M_PI);
    cairo_arc(cr, x + r, y + r, r, M_PI, 3*M_PI/2);
    cairo_close_path(cr);
    stroke_shape(cr, fill_white);
}

void draw_sun(cairo_t* cr, double x, double y, double r = 36.0) {
    cairo_save(cr);
    cairo_set_line_width(cr, 4.0);
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
    cairo_new_sub_path(cr);
    cairo_arc(cr, x, y, r, 0, 2 * M_PI);
    stroke_shape(cr, true);

    // Smiling face
    cairo_set_source_rgb(cr, 0.08, 0.08, 0.08);
    cairo_arc(cr, x - r*0.35, y - r*0.1, 4.0, 0, 2 * M_PI);
    cairo_arc(cr, x + r*0.35, y - r*0.1, 4.0, 0, 2 * M_PI);
    cairo_fill(cr);
    cairo_arc(cr, x, y + r*0.1, r*0.45, 0.2 * M_PI, 0.8 * M_PI);
    cairo_stroke(cr);

    // Rays
    for (int i = 0; i < 8; ++i) {
        double a = i * (M_PI / 4.0);
        cairo_move_to(cr, x + (r + 8) * std::cos(a), y + (r + 8) * std::sin(a));
        cairo_line_to(cr, x + (r + 22) * std::cos(a), y + (r + 22) * std::sin(a));
    }
    cairo_stroke(cr);
    cairo_restore(cr);
}

void draw_heart(cairo_t* cr, double x, double y, double s = 26.0) {
    cairo_save(cr);
    cairo_set_source_rgb(cr, 0.08, 0.08, 0.08);
    cairo_new_sub_path(cr);
    cairo_move_to(cr, x, y);
    cairo_curve_to(cr, x - s*0.6, y - s*0.8, x - s*1.1, y - s*0.2, x, y + s*0.8);
    cairo_curve_to(cr, x + s*1.1, y - s*0.2, x + s*0.6, y - s*0.8, x, y);
    cairo_fill(cr);
    cairo_restore(cr);
}

void draw_sparkle(cairo_t* cr, double x, double y, double s = 16.0) {
    cairo_save(cr);
    cairo_set_source_rgb(cr, 0.08, 0.08, 0.08);
    cairo_set_line_width(cr, 3.5);
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
    cairo_move_to(cr, x - s, y); cairo_line_to(cr, x + s, y);
    cairo_move_to(cr, x, y - s); cairo_line_to(cr, x, y + s);
    cairo_stroke(cr);
    cairo_restore(cr);
}

void draw_ground(cairo_t* cr, bool indoor = false) {
    cairo_save(cr);
    cairo_set_source_rgb(cr, 0.08, 0.08, 0.08);
    cairo_set_line_width(cr, 4.5);
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
    cairo_move_to(cr, 30, GY);
    if (!indoor) {
        cairo_curve_to(cr, W * 0.3, GY - 5, W * 0.6, GY + 5, W - 30, GY);
        cairo_stroke(cr);
        // Grass tufts
        cairo_set_line_width(cr, 3.5);
        for (int gx : {90, 180, 520, 890, 980}) {
            cairo_move_to(cr, gx, GY); cairo_line_to(cr, gx - 10, GY - 18);
            cairo_move_to(cr, gx, GY); cairo_line_to(cr, gx + 10, GY - 16);
            cairo_stroke(cr);
        }
    } else {
        cairo_line_to(cr, W - 30, GY);
        cairo_stroke(cr);
        cairo_set_line_width(cr, 2.0);
        cairo_set_source_rgb(cr, 0.45, 0.45, 0.45);
        cairo_move_to(cr, 30, GY - 18);
        cairo_line_to(cr, W - 30, GY - 18);
        cairo_stroke(cr);
    }
    cairo_restore(cr);
}

// Draw Mít's standard head with solid white fill
void draw_mit_head(cairo_t* cr, double x, double y, double r = 68.0, bool happy = false) {
    cairo_save(cr);
    cairo_set_line_width(cr, 4.5);
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
    cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);

    // Solid Head
    cairo_new_sub_path(cr);
    cairo_arc(cr, x, y, r, 0, 2 * M_PI);
    stroke_shape(cr, true);

    // 3 Sprout hair tufts
    cairo_set_source_rgb(cr, 0.08, 0.08, 0.08);
    cairo_move_to(cr, x - 16, y - r);
    cairo_curve_to(cr, x - 22, y - r - 25, x - 30, y - r - 38, x - 26, y - r - 48);
    cairo_move_to(cr, x, y - r);
    cairo_curve_to(cr, x, y - r - 30, x, y - r - 45, x, y - r - 55);
    cairo_move_to(cr, x + 16, y - r);
    cairo_curve_to(cr, x + 22, y - r - 25, x + 30, y - r - 38, x + 26, y - r - 48);
    cairo_stroke(cr);

    // Eyes
    if (!happy) {
        cairo_arc(cr, x - 25, y - 6, 6.5, 0, 2*M_PI);
        cairo_arc(cr, x + 25, y - 6, 6.5, 0, 2*M_PI);
        cairo_fill(cr);
    } else {
        cairo_move_to(cr, x - 36, y - 2);
        cairo_curve_to(cr, x - 25, y - 16, x - 25, y - 16, x - 14, y - 2);
        cairo_move_to(cr, x + 14, y - 2);
        cairo_curve_to(cr, x + 25, y - 16, x + 25, y - 16, x + 36, y - 2);
        cairo_stroke(cr);
    }

    // Smile & Cheeks
    cairo_arc(cr, x, y + 16, 22.0, 0.15*M_PI, 0.85*M_PI); cairo_stroke(cr);
    cairo_arc(cr, x - 42, y + 14, 3.5, 0, 2*M_PI);
    cairo_arc(cr, x + 42, y + 14, 3.5, 0, 2*M_PI); cairo_fill(cr);

    cairo_restore(cr);
}

// Mít standing upright with solid body
void draw_mit_standing(cairo_t* cr, double x, bool happy = false) {
    cairo_save(cr);
    cairo_set_line_width(cr, 4.5);
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
    cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);

    double r_head = 68.0;
    double head_cy = GY - 270.0;
    double body_top = head_cy + r_head - 8.0;
    double body_bot = GY - 42.0;

    // Legs & Shoes firmly on GY
    double leg_l = x - 26.0, leg_r = x + 26.0;
    cairo_set_source_rgb(cr, 0.08, 0.08, 0.08);
    cairo_move_to(cr, leg_l, body_bot); cairo_line_to(cr, leg_l, GY - 8); cairo_stroke(cr);
    draw_rounded_rect(cr, leg_l - 14, GY - 16, 32, 16, 7.0, true);
    cairo_move_to(cr, leg_r, body_bot); cairo_line_to(cr, leg_r, GY - 8); cairo_stroke(cr);
    draw_rounded_rect(cr, leg_r - 14, GY - 16, 32, 16, 7.0, true);

    // Tunic (solid white fill blocks any background line)
    cairo_move_to(cr, x - 40, body_top); cairo_line_to(cr, x + 40, body_top);
    cairo_line_to(cr, x + 60, body_bot); cairo_line_to(cr, x - 60, body_bot);
    cairo_close_path(cr);
    stroke_shape(cr, true);

    // Collar
    cairo_set_source_rgb(cr, 0.08, 0.08, 0.08);
    cairo_arc(cr, x, body_top, 20.0, 0, M_PI); cairo_stroke(cr);

    draw_mit_head(cr, x, head_cy, r_head, happy);
    cairo_restore(cr);
}

// Friend Mai with two cute side pigtail buns
void draw_friend_standing(cairo_t* cr, double x, bool happy = false) {
    cairo_save(cr);
    cairo_set_line_width(cr, 4.5);
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
    cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);

    double r_head = 68.0;
    double head_cy = GY - 270.0;
    double body_top = head_cy + r_head - 8.0;
    double body_bot = GY - 42.0;

    // Legs & Shoes
    double leg_l = x - 26.0, leg_r = x + 26.0;
    cairo_set_source_rgb(cr, 0.08, 0.08, 0.08);
    cairo_move_to(cr, leg_l, body_bot); cairo_line_to(cr, leg_l, GY - 8); cairo_stroke(cr);
    draw_rounded_rect(cr, leg_l - 14, GY - 16, 32, 16, 7.0, true);
    cairo_move_to(cr, leg_r, body_bot); cairo_line_to(cr, leg_r, GY - 8); cairo_stroke(cr);
    draw_rounded_rect(cr, leg_r - 14, GY - 16, 32, 16, 7.0, true);

    // Dress
    cairo_move_to(cr, x - 40, body_top); cairo_line_to(cr, x + 40, body_top);
    cairo_line_to(cr, x + 60, body_bot); cairo_line_to(cr, x - 60, body_bot);
    cairo_close_path(cr);
    stroke_shape(cr, true);
    cairo_set_source_rgb(cr, 0.08, 0.08, 0.08);
    cairo_arc(cr, x, body_top, 20.0, 0, M_PI); cairo_stroke(cr);

    // Head
    cairo_arc(cr, x, head_cy, r_head, 0, 2 * M_PI);
    stroke_shape(cr, true);

    // Two cute round pigtail buns (solid white fill + stroke)
    cairo_arc(cr, x - r_head - 8, head_cy - 15, 24, 0, 2 * M_PI); stroke_shape(cr, true);
    cairo_arc(cr, x + r_head + 8, head_cy - 15, 24, 0, 2 * M_PI); stroke_shape(cr, true);

    // Eyes
    cairo_set_source_rgb(cr, 0.08, 0.08, 0.08);
    if (!happy) {
        cairo_arc(cr, x - 25, head_cy - 6, 6.5, 0, 2*M_PI);
        cairo_arc(cr, x + 25, head_cy - 6, 6.5, 0, 2*M_PI);
        cairo_fill(cr);
    } else {
        cairo_move_to(cr, x - 36, head_cy - 2);
        cairo_curve_to(cr, x - 25, head_cy - 16, x - 25, head_cy - 16, x - 14, head_cy - 2);
        cairo_move_to(cr, x + 14, head_cy - 2);
        cairo_curve_to(cr, x + 25, head_cy - 16, x + 25, head_cy - 16, x + 36, head_cy - 2);
        cairo_stroke(cr);
    }
    cairo_arc(cr, x, head_cy + 16, 22.0, 0.15*M_PI, 0.85*M_PI); cairo_stroke(cr);
    cairo_arc(cr, x - 42, head_cy + 14, 3.5, 0, 2*M_PI);
    cairo_arc(cr, x + 42, head_cy + 14, 3.5, 0, 2*M_PI); cairo_fill(cr);

    cairo_restore(cr);
}

// ==============================================================================
// 16 DEDICATED SCENE BUILDERS (ZERO OVERLAP & PURE CLARITY)
// ==============================================================================

// PAGE 1: Chào ông bà (ch - chào)
void render_scene_1(cairo_t* cr, bool is_bloom) {
    draw_ground(cr, true);
    double mx = 340.0, gx = 750.0;

    // 1. Armchair in background
    cairo_set_line_width(cr, 4.5);
    draw_rounded_rect(cr, gx - 95, GY - 230, 190, 230, 22.0, true);

    // 2. Grandma body (solid white fill over armchair)
    double g_head_y = GY - 245.0;
    cairo_move_to(cr, gx - 50, GY - 170); cairo_line_to(cr, gx + 50, GY - 170);
    cairo_line_to(cr, gx + 65, GY); cairo_line_to(cr, gx - 65, GY);
    cairo_close_path(cr); stroke_shape(cr, true);

    // 3. Grandma head & bun
    cairo_arc(cr, gx - 52, g_head_y - 12, 26, 0, 2*M_PI); stroke_shape(cr, true); // Bun
    cairo_arc(cr, gx, g_head_y, 60, 0, 2*M_PI); stroke_shape(cr, true); // Head
    // Eyeglasses
    cairo_set_source_rgb(cr, 0.08, 0.08, 0.08);
    cairo_arc(cr, gx - 18, g_head_y - 8, 14, 0, 2*M_PI); cairo_stroke(cr);
    cairo_arc(cr, gx + 18, g_head_y - 8, 14, 0, 2*M_PI); cairo_stroke(cr);
    cairo_move_to(cr, gx - 4, g_head_y - 8); cairo_line_to(cr, gx + 4, g_head_y - 8); cairo_stroke(cr);
    cairo_arc(cr, gx, g_head_y + 18, 20, 0.2*M_PI, 0.8*M_PI); cairo_stroke(cr);

    // 4. Mít standing bowing politely
    draw_mit_standing(cr, mx, is_bloom);
    cairo_set_line_width(cr, 4.5);
    cairo_move_to(cr, mx - 35, GY - 170);
    cairo_line_to(cr, mx + 20, GY - 130);
    cairo_line_to(cr, mx + 45, GY - 145);
    cairo_stroke(cr);

    if (is_bloom) {
        cairo_move_to(cr, gx - 45, GY - 150);
        cairo_curve_to(cr, gx - 160, GY - 220, mx + 60, GY - 260, mx + 15, GY - 275);
        cairo_stroke(cr);
        draw_heart(cr, (mx + gx)/2.0, GY - 300, 30);
        draw_sun(cr, 130, 130, 38);
    }
}

// PAGE 2: Nhặt rau giúp mẹ (au - rau)
void render_scene_2(cairo_t* cr, bool is_bloom) {
    draw_ground(cr, true);
    double ox = 320.0, mx = 740.0;

    // Mom on stool
    cairo_set_line_width(cr, 4.5);
    draw_rounded_rect(cr, ox - 45, GY - 95, 90, 95, 12.0, true);
    double mom_head_y = GY - 265.0;
    cairo_arc(cr, ox - 52, mom_head_y - 10, 24, 0, 2*M_PI); stroke_shape(cr, true); // Ponytail
    cairo_arc(cr, ox, mom_head_y, 60, 0, 2*M_PI); stroke_shape(cr, true); // Head
    cairo_set_source_rgb(cr, 0.08, 0.08, 0.08);
    cairo_arc(cr, ox + 16, mom_head_y - 8, 5.5, 0, 2*M_PI); cairo_fill(cr);
    cairo_arc(cr, ox + 16, mom_head_y + 16, 20, 0.2*M_PI, 0.8*M_PI); cairo_stroke(cr);

    // Mom dress
    cairo_move_to(cr, ox - 40, mom_head_y + 55); cairo_line_to(cr, ox + 40, mom_head_y + 55);
    cairo_line_to(cr, ox + 55, GY - 95); cairo_line_to(cr, ox - 55, GY - 95);
    cairo_close_path(cr); stroke_shape(cr, true);

    // Large basket of fresh vegetables in center
    double bx = 530.0;
    cairo_arc(cr, bx, GY - 45, 65, 0, M_PI); cairo_close_path(cr); stroke_shape(cr, true);
    for (double lx : {bx - 36, bx, bx + 36}) {
        cairo_arc(cr, lx, GY - 62, 20, 0, 2*M_PI); stroke_shape(cr, true);
    }

    // Mít standing helping
    draw_mit_standing(cr, mx, is_bloom);
    cairo_set_line_width(cr, 4.5);
    cairo_move_to(cr, mx - 35, GY - 170); cairo_line_to(cr, bx + 40, GY - 65); cairo_stroke(cr);

    if (is_bloom) {
        draw_sparkle(cr, bx, GY - 110, 18);
        draw_heart(cr, bx, GY - 280, 28);
        draw_sun(cr, W - 130, 130, 38);
    }
}

// PAGE 3: Cho mèo ăn cơm (ơm - cơm)
void render_scene_3(cairo_t* cr, bool is_bloom) {
    draw_ground(cr, true);
    double mx = 360.0, cx = 740.0;

    // Cat body
    cairo_set_line_width(cr, 4.5);
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
    cairo_arc(cr, cx, GY - 60, 56, 0, 2*M_PI); stroke_shape(cr, true);
    // Ears
    cairo_move_to(cr, cx - 36, GY - 105); cairo_line_to(cr, cx - 48, GY - 150); cairo_line_to(cr, cx - 14, GY - 115); cairo_stroke(cr);
    cairo_move_to(cr, cx + 36, GY - 105); cairo_line_to(cr, cx + 48, GY - 150); cairo_line_to(cr, cx + 14, GY - 115); cairo_stroke(cr);
    // Whiskers
    cairo_move_to(cr, cx - 20, GY - 55); cairo_line_to(cr, cx - 60, GY - 60); cairo_stroke(cr);
    cairo_move_to(cr, cx - 20, GY - 45); cairo_line_to(cr, cx - 60, GY - 42); cairo_stroke(cr);
    cairo_move_to(cr, cx + 20, GY - 55); cairo_line_to(cr, cx + 60, GY - 60); cairo_stroke(cr);
    cairo_move_to(cr, cx + 20, GY - 45); cairo_line_to(cr, cx + 60, GY - 42); cairo_stroke(cr);
    // Tail
    cairo_move_to(cr, cx + 45, GY - 40);
    cairo_curve_to(cr, cx + 95, GY - 60, cx + 110, GY - 120, cx + 80, GY - 130); cairo_stroke(cr);

    // Rice bowl resting cleanly ON GY
    double bowl_x = 540.0;
    cairo_move_to(cr, bowl_x - 44, GY - 44); cairo_line_to(cr, bowl_x + 44, GY - 44);
    cairo_line_to(cr, bowl_x + 32, GY); cairo_line_to(cr, bowl_x - 32, GY);
    cairo_close_path(cr); stroke_shape(cr, true);
    cairo_arc(cr, bowl_x, GY - 44, 36, M_PI, 2*M_PI); stroke_shape(cr, true); // Rice mound

    // Mít standing offering bowl
    draw_mit_standing(cr, mx, is_bloom);
    cairo_move_to(cr, mx + 35, GY - 160); cairo_line_to(cr, bowl_x - 20, GY - 45); cairo_stroke(cr);

    if (!is_bloom) {
        cairo_arc(cr, cx - 18, GY - 65, 6.0, 0, 2*M_PI);
        cairo_arc(cr, cx + 18, GY - 65, 6.0, 0, 2*M_PI); cairo_fill(cr);
    } else {
        cairo_move_to(cr, cx - 28, GY - 65); cairo_curve_to(cr, cx - 18, GY - 78, cx - 18, GY - 78, cx - 8, GY - 65);
        cairo_move_to(cr, cx + 8, GY - 65);  cairo_curve_to(cr, cx + 18, GY - 78, cx + 18, GY - 78, cx + 28, GY - 65);
        cairo_stroke(cr);
        draw_heart(cr, cx, GY - 160, 28);
        draw_heart(cr, bowl_x, GY - 100, 22);
    }
}

// PAGE 4: Nhặt rác bỏ thùng (r - rác)
void render_scene_4(cairo_t* cr, bool is_bloom) {
    draw_ground(cr, false);
    double mx = 340.0, bx = 760.0;

    // Clean trash bin
    cairo_set_line_width(cr, 4.5);
    draw_rounded_rect(cr, bx - 55, GY - 170, 110, 170, 14.0, true);
    draw_rounded_rect(cr, bx - 65, GY - 195, 130, 25, 8.0, true);
    cairo_set_line_width(cr, 3.0);
    cairo_arc(cr, bx, GY - 90, 25, 0, 1.8*M_PI); cairo_stroke(cr);

    if (!is_bloom) {
        // Candy wrapper with twisted ends
        double rx = 530.0;
        cairo_set_line_width(cr, 3.5);
        draw_rounded_rect(cr, rx - 18, GY - 22, 36, 22, 6.0, true);
        cairo_move_to(cr, rx - 18, GY - 11); cairo_line_to(cr, rx - 32, GY - 20); cairo_line_to(cr, rx - 32, GY - 2); cairo_close_path(cr); cairo_stroke(cr);
        cairo_move_to(cr, rx + 18, GY - 11); cairo_line_to(cr, rx + 32, GY - 20); cairo_line_to(cr, rx + 32, GY - 2); cairo_close_path(cr); cairo_stroke(cr);

        // Mít reaching arm
        draw_mit_standing(cr, mx, false);
        cairo_set_line_width(cr, 4.5);
        cairo_move_to(cr, mx + 35, GY - 160);
        cairo_curve_to(cr, mx + 100, GY - 130, rx - 40, GY - 80, rx - 10, GY - 28);
        cairo_stroke(cr);
    } else {
        draw_mit_standing(cr, mx, true);
        cairo_set_line_width(cr, 4.5);
        cairo_move_to(cr, mx - 35, GY - 160); cairo_line_to(cr, mx - 70, GY - 210);
        cairo_move_to(cr, mx + 35, GY - 160); cairo_line_to(cr, mx + 70, GY - 210);
        cairo_stroke(cr);

        draw_sparkle(cr, 520, GY - 40, 20);
        draw_sparkle(cr, 630, GY - 30, 16);
        draw_sun(cr, 130, 130, 38);
    }
}

// PAGE 5: Tưới cây cho bà (t - tưới)
void render_scene_5(cairo_t* cr, bool is_bloom) {
    draw_ground(cr, false);
    double mx = 340.0, px = 760.0;

    // Plant pot
    cairo_set_line_width(cr, 4.5);
    cairo_move_to(cr, px - 65, GY - 110); cairo_line_to(cr, px + 65, GY - 110);
    cairo_line_to(cr, px + 48, GY); cairo_line_to(cr, px - 48, GY);
    cairo_close_path(cr); stroke_shape(cr, true);
    draw_rounded_rect(cr, px - 72, GY - 128, 144, 18, 6.0, true);

    // Stem
    cairo_move_to(cr, px, GY - 128);
    cairo_curve_to(cr, px - 20, GY - 220, px + 20, GY - 290, px, GY - 350);
    cairo_stroke(cr);

    // Mít with watering can
    draw_mit_standing(cr, mx, is_bloom);
    double can_x = mx + 130, can_y = GY - 150;
    draw_rounded_rect(cr, can_x - 36, can_y - 28, 72, 56, 12.0, true);
    cairo_move_to(cr, can_x - 36, can_y); cairo_line_to(cr, can_x - 60, can_y - 35); cairo_stroke(cr);
    cairo_move_to(cr, can_x + 36, can_y); cairo_line_to(cr, can_x + 85, can_y - 25); cairo_stroke(cr);
    cairo_move_to(cr, mx + 35, GY - 160); cairo_line_to(cr, can_x - 45, can_y - 25); cairo_stroke(cr);

    if (!is_bloom) {
        cairo_arc(cr, px - 20, GY - 340, 22, 0, M_PI); cairo_stroke(cr);
        cairo_arc(cr, can_x + 110, can_y - 10, 5, 0, 2*M_PI); cairo_fill(cr);
        cairo_arc(cr, can_x + 135, can_y + 15, 5, 0, 2*M_PI); cairo_fill(cr);
    } else {
        for (int i = 0; i < 6; ++i) {
            double a = i * (M_PI / 3.0);
            cairo_arc(cr, px + 44 * std::cos(a), (GY - 350) + 44 * std::sin(a), 24, 0, 2*M_PI);
            stroke_shape(cr, true);
        }
        cairo_arc(cr, px, GY - 350, 20, 0, 2*M_PI); stroke_shape(cr, true);
        draw_sun(cr, 130, 130, 38);
        draw_heart(cr, (mx + px)/2.0, GY - 280, 28);
    }
}

// PAGE 6: Xếp dép ngay ngắn (d - dép)
void render_scene_6(cairo_t* cr, bool is_bloom) {
    draw_ground(cr, true);
    double step_x = 640.0;
    double mx = 340.0;

    // Step platform
    cairo_set_line_width(cr, 4.5);
    draw_rounded_rect(cr, step_x - 30, GY - 40, 300, 40, 8.0, true);

    if (!is_bloom) {
        // Messy sandals with Y-straps
        draw_rounded_rect(cr, step_x + 30, GY - 35, 46, 85, 16.0, true);
        cairo_move_to(cr, step_x + 53, GY - 10); cairo_line_to(cr, step_x + 35, GY + 25); cairo_stroke(cr);
        cairo_move_to(cr, step_x + 53, GY - 10); cairo_line_to(cr, step_x + 71, GY + 25); cairo_stroke(cr);

        cairo_save(cr);
        cairo_translate(cr, step_x + 150, GY - 10);
        cairo_rotate(cr, 0.5);
        draw_rounded_rect(cr, -23, -42, 46, 85, 16.0, true);
        cairo_restore(cr);

        draw_mit_standing(cr, mx, false);
        cairo_set_line_width(cr, 4.5);
        cairo_move_to(cr, mx + 35, GY - 160); cairo_line_to(cr, step_x + 15, GY - 30); cairo_stroke(cr);
    } else {
        // Symmetrical neat slippers
        draw_rounded_rect(cr, step_x + 50, GY - 35, 46, 85, 16.0, true);
        cairo_move_to(cr, step_x + 73, GY - 10); cairo_line_to(cr, step_x + 55, GY + 25); cairo_stroke(cr);
        cairo_move_to(cr, step_x + 73, GY - 10); cairo_line_to(cr, step_x + 91, GY + 25); cairo_stroke(cr);

        draw_rounded_rect(cr, step_x + 120, GY - 35, 46, 85, 16.0, true);
        cairo_move_to(cr, step_x + 143, GY - 10); cairo_line_to(cr, step_x + 125, GY + 25); cairo_stroke(cr);
        cairo_move_to(cr, step_x + 143, GY - 10); cairo_line_to(cr, step_x + 161, GY + 25); cairo_stroke(cr);

        draw_mit_standing(cr, mx, true);
        cairo_set_line_width(cr, 4.5);
        cairo_move_to(cr, mx - 35, GY - 160); cairo_line_to(cr, mx - 60, GY - 130); cairo_line_to(cr, mx - 35, GY - 110);
        cairo_move_to(cr, mx + 35, GY - 160); cairo_line_to(cr, mx + 60, GY - 130); cairo_line_to(cr, mx + 35, GY - 110);
        cairo_stroke(cr);

        draw_sparkle(cr, step_x + 200, GY - 25, 20);
        draw_heart(cr, (mx + step_x)/2.0, GY - 280, 28);
    }
}

// PAGE 7: Chia quả táo (áo - táo)
void render_scene_7(cairo_t* cr, bool is_bloom) {
    draw_ground(cr, false);
    double mx = 400.0, fx = 680.0;

    draw_friend_standing(cr, fx, is_bloom);
    draw_mit_standing(cr, mx, is_bloom);

    double mid_x = (mx + fx) / 2.0;

    if (!is_bloom) {
        cairo_set_line_width(cr, 4.5);
        cairo_arc(cr, mid_x, GY - 150, 36, 0, 2*M_PI); stroke_shape(cr, true);
        cairo_move_to(cr, mid_x, GY - 186); cairo_line_to(cr, mid_x, GY - 205); cairo_stroke(cr);
        cairo_arc(cr, mid_x + 14, GY - 198, 10, 0, 2*M_PI); stroke_shape(cr, true);
        cairo_move_to(cr, mx + 35, GY - 160); cairo_line_to(cr, mid_x - 36, GY - 150); cairo_stroke(cr);
        cairo_move_to(cr, fx - 35, GY - 160); cairo_line_to(cr, mid_x + 36, GY - 150); cairo_stroke(cr);
    } else {
        cairo_set_line_width(cr, 4.5);
        cairo_arc(cr, mx + 50, GY - 150, 26, -M_PI/2, M_PI/2); cairo_close_path(cr); stroke_shape(cr, true);
        cairo_arc(cr, fx - 50, GY - 150, 26, M_PI/2, 3*M_PI/2); cairo_close_path(cr); stroke_shape(cr, true);
        draw_heart(cr, mid_x, GY - 280, 30);
        draw_sun(cr, 130, 130, 38);
    }
}

// PAGE 8: Dắt tay em nhỏ (em - em)
void render_scene_8(cairo_t* cr, bool is_bloom) {
    draw_ground(cr, false);
    double mx = 380.0, bx = 690.0;

    draw_mit_standing(cr, mx, is_bloom);

    // Chubby toddler
    cairo_set_line_width(cr, 4.5);
    double b_head_cy = GY - 180.0;
    cairo_arc(cr, bx, b_head_cy, 48, 0, 2*M_PI); stroke_shape(cr, true);
    cairo_move_to(cr, bx, b_head_cy - 48); cairo_line_to(cr, bx, b_head_cy - 72); cairo_stroke(cr);
    cairo_set_source_rgb(cr, 0.08, 0.08, 0.08);
    cairo_arc(cr, bx - 16, b_head_cy - 6, 4.5, 0, 2*M_PI);
    cairo_arc(cr, bx + 16, b_head_cy - 6, 4.5, 0, 2*M_PI); cairo_fill(cr);
    cairo_arc(cr, bx, b_head_cy + 14, 16, 0.2*M_PI, 0.8*M_PI); cairo_stroke(cr);

    draw_rounded_rect(cr, bx - 38, GY - 125, 76, 85, 20.0, true);
    draw_rounded_rect(cr, bx - 32, GY - 16, 26, 16, 6.0, true);
    draw_rounded_rect(cr, bx + 6, GY - 16, 26, 16, 6.0, true);

    cairo_set_line_width(cr, 4.5);
    cairo_move_to(cr, mx + 35, GY - 160);
    cairo_curve_to(cr, (mx+bx)/2.0, GY - 120, (mx+bx)/2.0, GY - 110, bx - 35, GY - 90);
    cairo_stroke(cr);

    if (is_bloom) {
        draw_heart(cr, (mx + bx)/2.0, GY - 280, 28);
        draw_sparkle(cr, bx + 55, b_head_cy - 20, 16);
        draw_sun(cr, 130, 130, 38);
    }
}

// PAGE 9: Lau sạch chiếc bàn (b - bàn)
void render_scene_9(cairo_t* cr, bool is_bloom) {
    draw_ground(cr, true);
    double mx = 340.0, dx = 720.0;

    // Study desk
    cairo_set_line_width(cr, 4.5);
    draw_rounded_rect(cr, dx - 140, GY - 170, 280, 35, 8.0, true);
    cairo_move_to(cr, dx - 110, GY - 135); cairo_line_to(cr, dx - 110, GY); cairo_stroke(cr);
    cairo_move_to(cr, dx + 110, GY - 135); cairo_line_to(cr, dx + 110, GY); cairo_stroke(cr);
    draw_rounded_rect(cr, dx + 75, GY - 225, 36, 55, 6.0, true);
    cairo_move_to(cr, dx + 85, GY - 225); cairo_line_to(cr, dx + 78, GY - 260); cairo_stroke(cr);
    cairo_move_to(cr, dx + 98, GY - 225); cairo_line_to(cr, dx + 105, GY - 260); cairo_stroke(cr);

    draw_mit_standing(cr, mx, is_bloom);
    cairo_set_line_width(cr, 4.5);
    cairo_move_to(cr, mx + 35, GY - 160); cairo_line_to(cr, dx - 40, GY - 180); cairo_stroke(cr);
    draw_rounded_rect(cr, dx - 60, GY - 198, 40, 28, 8.0, true);

    if (!is_bloom) {
        cairo_set_line_width(cr, 2.5);
        cairo_move_to(cr, dx - 90, GY - 152); cairo_line_to(cr, dx - 60, GY - 146);
        cairo_line_to(cr, dx - 30, GY - 155); cairo_stroke(cr);
    } else {
        draw_sparkle(cr, dx - 60, GY - 152, 20);
        draw_sparkle(cr, dx + 10, GY - 152, 22);
        draw_heart(cr, mx + 80, GY - 280, 28);
    }
}

// PAGE 10: Hái lá thơm cho bà (l - lá)
void render_scene_10(cairo_t* cr, bool is_bloom) {
    draw_ground(cr, true);
    double mx = 340.0, gx = 750.0;

    // Armchair
    cairo_set_line_width(cr, 4.5);
    draw_rounded_rect(cr, gx - 95, GY - 230, 190, 230, 22.0, true);
    double g_head_y = GY - 245.0;
    cairo_arc(cr, gx - 52, g_head_y - 12, 26, 0, 2*M_PI); stroke_shape(cr, true);
    cairo_arc(cr, gx, g_head_y, 60, 0, 2*M_PI); stroke_shape(cr, true);

    // Mít holding basket
    draw_mit_standing(cr, mx, is_bloom);
    double bsk_x = mx + 120, bsk_y = GY - 140;
    cairo_arc(cr, bsk_x, bsk_y, 40, 0, M_PI); cairo_close_path(cr); stroke_shape(cr, true);
    cairo_arc(cr, bsk_x - 18, bsk_y - 12, 16, 0, 2*M_PI); stroke_shape(cr, true);
    cairo_arc(cr, bsk_x + 18, bsk_y - 12, 16, 0, 2*M_PI); stroke_shape(cr, true);
    cairo_move_to(cr, mx + 35, GY - 160); cairo_line_to(cr, bsk_x - 30, bsk_y); cairo_stroke(cr);

    if (is_bloom) {
        double tx = gx - 125, ty = GY - 70;
        draw_rounded_rect(cr, tx - 30, ty, 60, 70, 6.0, true);
        draw_rounded_rect(cr, tx - 18, ty - 30, 36, 30, 6.0, true);
        cairo_move_to(cr, tx - 6, ty - 38); cairo_curve_to(cr, tx - 12, ty - 60, tx, ty - 70, tx - 6, ty - 90);
        cairo_move_to(cr, tx + 6, ty - 38); cairo_curve_to(cr, tx, ty - 60, tx + 12, ty - 70, tx + 6, ty - 90);
        cairo_stroke(cr);
        draw_heart(cr, (mx + gx)/2.0, GY - 280, 28);
    }
}

// PAGE 11: Che ô khi trời mưa (ưa - mưa)
void render_scene_11(cairo_t* cr, bool is_bloom) {
    draw_ground(cr, false);
    double mx = 400.0, fx = 680.0;

    // 1. Rain lines in background
    if (!is_bloom) {
        cairo_save(cr);
        cairo_set_source_rgb(cr, 0.25, 0.25, 0.25);
        cairo_set_line_width(cr, 3.0);
        for (double rx : {90.0, 160.0, 880.0, 960.0}) {
            for (double ry : {120.0, 240.0, 360.0}) {
                cairo_move_to(cr, rx, ry); cairo_line_to(cr, rx - 16, ry + 32); cairo_stroke(cr);
            }
        }
        cairo_restore(cr);
    }

    // 2. Umbrella high up (solid white fill hides any rain behind it)
    double ux = (mx + fx) / 2.0;
    double uy = GY - 390.0;
    cairo_set_line_width(cr, 5.0);
    cairo_arc(cr, ux, uy, 160, M_PI, 2*M_PI); cairo_close_path(cr); stroke_shape(cr, true);
    cairo_move_to(cr, ux, uy - 160); cairo_line_to(cr, ux, uy - 180); cairo_stroke(cr); // Tip
    cairo_move_to(cr, ux, uy); cairo_line_to(cr, mx + 35, GY - 160); cairo_stroke(cr); // Handle

    // 3. Children standing dry under umbrella
    draw_mit_standing(cr, mx, is_bloom);
    draw_friend_standing(cr, fx, is_bloom);

    if (is_bloom) {
        draw_sun(cr, 130, 130, 38);
        draw_heart(cr, ux, uy - 50, 30);
    }
}

// PAGE 12: Giữ yên lặng cho em ngủ (ng - ngủ)
void render_scene_12(cairo_t* cr, bool is_bloom) {
    draw_ground(cr, true);
    double mx = 320.0, cx = 740.0;

    // Crib sitting ON ground GY
    cairo_set_line_width(cr, 4.5);
    draw_rounded_rect(cr, cx - 130, GY - 180, 260, 180, 16.0, true);
    for (int bar = -90; bar <= 90; bar += 45) {
        cairo_move_to(cr, cx + bar, GY - 180); cairo_line_to(cr, cx + bar, GY); cairo_stroke(cr);
    }
    // Baby head resting on pillow
    cairo_arc(cr, cx - 45, GY - 195, 28, 0, 2*M_PI); stroke_shape(cr, true);
    cairo_set_source_rgb(cr, 0.08, 0.08, 0.08);
    cairo_arc(cr, cx - 45, GY - 195, 10, 0.2*M_PI, 0.8*M_PI); cairo_stroke(cr);

    // Mít tiptoeing on left with finger on lips "shh"
    draw_mit_standing(cr, mx, false);
    cairo_set_line_width(cr, 4.5);
    cairo_move_to(cr, mx + 35, GY - 160);
    cairo_line_to(cr, mx + 15, GY - 230);
    cairo_line_to(cr, mx + 15, GY - 255);
    cairo_stroke(cr);

    if (!is_bloom) {
        cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_ITALIC, CAIRO_FONT_WEIGHT_BOLD);
        cairo_set_font_size(cr, 26.0); cairo_move_to(cr, cx + 20, GY - 210); cairo_show_text(cr, "z");
        cairo_set_font_size(cr, 36.0); cairo_move_to(cr, cx + 50, GY - 250); cairo_show_text(cr, "Z");
        cairo_set_font_size(cr, 48.0); cairo_move_to(cr, cx + 85, GY - 295); cairo_show_text(cr, "Z");
    } else {
        cairo_arc(cr, 140, 140, 40, 0.3*M_PI, 1.7*M_PI); cairo_stroke(cr);
        cairo_arc(cr, 162, 140, 32, 0.5*M_PI, 1.5*M_PI); cairo_stroke(cr);
        draw_sparkle(cr, 230, 110, 16);
        draw_sparkle(cr, 290, 150, 12);
    }
}

// PAGE 13: Bắt sâu cho hoa (âu - sâu)
void render_scene_13(cairo_t* cr, bool is_bloom) {
    draw_ground(cr, false);
    double mx = 340.0, px = 760.0;

    cairo_set_line_width(cr, 4.5);
    cairo_move_to(cr, px, GY);
    cairo_curve_to(cr, px - 25, GY - 100, px + 20, GY - 200, px, GY - 300);
    cairo_stroke(cr);
    cairo_arc(cr, px - 45, GY - 180, 35, 0.2*M_PI, 1.4*M_PI); stroke_shape(cr, true);
    cairo_arc(cr, px + 45, GY - 240, 35, -0.4*M_PI, 0.8*M_PI); stroke_shape(cr, true);

    draw_mit_standing(cr, mx, is_bloom);
    cairo_move_to(cr, mx + 35, GY - 160); cairo_line_to(cr, px - 45, GY - 180); cairo_stroke(cr);

    if (!is_bloom) {
        for (int i = 0; i < 4; ++i) {
            cairo_arc(cr, px - 70 + i*14, GY - 195, 9.0, 0, 2*M_PI); stroke_shape(cr, true);
        }
    } else {
        for (int i = 0; i < 4; ++i) {
            cairo_arc(cr, 520 + i*14, GY - 14, 9.0, 0, 2*M_PI); stroke_shape(cr, true);
        }
        for (int i = 0; i < 6; ++i) {
            double a = i * (M_PI / 3.0);
            cairo_arc(cr, px + 38 * std::cos(a), (GY - 300) + 38 * std::sin(a), 22, 0, 2*M_PI);
            stroke_shape(cr, true);
        }
        cairo_arc(cr, px, GY - 300, 18, 0, 2*M_PI); stroke_shape(cr, true);
        draw_sun(cr, 130, 130, 38);
    }
}

// PAGE 14: Rót nước mời ông (ước - nước)
void render_scene_14(cairo_t* cr, bool is_bloom) {
    draw_ground(cr, true);
    double mx = 340.0, gx = 750.0;

    // Armchair
    cairo_set_line_width(cr, 4.5);
    draw_rounded_rect(cr, gx - 95, GY - 230, 190, 230, 22.0, true);
    double g_head_y = GY - 245.0;
    cairo_arc(cr, gx, g_head_y, 60, 0, 2*M_PI); stroke_shape(cr, true);
    cairo_set_source_rgb(cr, 0.08, 0.08, 0.08);
    cairo_arc(cr, gx - 18, g_head_y - 8, 14, 0, 2*M_PI); cairo_stroke(cr);
    cairo_arc(cr, gx + 18, g_head_y - 8, 14, 0, 2*M_PI); cairo_stroke(cr);
    cairo_move_to(cr, gx - 4, g_head_y - 8); cairo_line_to(cr, gx + 4, g_head_y - 8); cairo_stroke(cr);
    cairo_move_to(cr, gx + 95, GY - 180); cairo_line_to(cr, gx + 95, GY); cairo_stroke(cr);
    cairo_arc(cr, gx + 82, GY - 180, 13, M_PI, 2*M_PI); cairo_stroke(cr);

    // Mít holding cup with BOTH hands
    draw_mit_standing(cr, mx, is_bloom);
    double cup_x = mx + 110, cup_y = GY - 150;
    draw_rounded_rect(cr, cup_x - 22, cup_y - 25, 44, 40, 8.0, true);
    cairo_move_to(cr, mx + 35, GY - 170); cairo_line_to(cr, cup_x - 22, cup_y - 8); cairo_stroke(cr);
    cairo_move_to(cr, mx + 35, GY - 145); cairo_line_to(cr, cup_x - 22, cup_y + 8); cairo_stroke(cr);

    if (is_bloom) {
        cairo_move_to(cr, cup_x - 6, cup_y - 32); cairo_curve_to(cr, cup_x - 12, cup_y - 55, cup_x, cup_y - 65, cup_x - 6, cup_y - 85);
        cairo_move_to(cr, cup_x + 6, cup_y - 32); cairo_curve_to(cr, cup_x, cup_y - 55, cup_x + 12, cup_y - 65, cup_x + 6, cup_y - 85);
        cairo_stroke(cr);
        draw_heart(cr, (mx + gx)/2.0, GY - 280, 28);
    }
}

// PAGE 15: Cất đồ chơi gọn gàng (g - gọn)
void render_scene_15(cairo_t* cr, bool is_bloom) {
    draw_ground(cr, true);
    double mx = 340.0, bx = 760.0;

    // Toy chest
    cairo_set_line_width(cr, 4.5);
    draw_rounded_rect(cr, bx - 80, GY - 140, 160, 140, 18.0, true);

    if (!is_bloom) {
        cairo_set_line_width(cr, 4.0);
        draw_rounded_rect(cr, 500, GY - 42, 42, 42, 6.0, true);
        cairo_move_to(cr, 580, GY); cairo_line_to(cr, 605, GY - 48); cairo_line_to(cr, 630, GY); cairo_close_path(cr); stroke_shape(cr, true);
        cairo_arc(cr, 680, GY - 22, 22, 0, 2*M_PI); stroke_shape(cr, true);

        draw_mit_standing(cr, mx, false);
        cairo_move_to(cr, mx + 35, GY - 160); cairo_line_to(cr, bx - 50, GY - 100); cairo_stroke(cr);
        draw_rounded_rect(cr, bx - 70, GY - 118, 32, 32, 5.0, true);
    } else {
        draw_rounded_rect(cr, bx - 55, GY - 165, 36, 36, 5.0, true);
        cairo_arc(cr, bx + 25, GY - 150, 18, 0, 2*M_PI); stroke_shape(cr, true);

        draw_mit_standing(cr, mx, true);
        cairo_set_line_width(cr, 4.5);
        cairo_move_to(cr, mx - 35, GY - 160); cairo_line_to(cr, mx - 60, GY - 130); cairo_line_to(cr, mx - 35, GY - 110);
        cairo_move_to(cr, mx + 35, GY - 160); cairo_line_to(cr, mx + 60, GY - 130); cairo_line_to(cr, mx + 35, GY - 110);
        cairo_stroke(cr);

        draw_sparkle(cr, 510, GY - 30, 20);
        draw_sparkle(cr, 620, GY - 25, 18);
        draw_heart(cr, (mx + bx)/2.0, GY - 280, 28);
    }
}

// PAGE 16: Ngôi nhà ấm cúng (nh - nhà)
void render_scene_16(cairo_t* cr, bool is_bloom) {
    if (!is_bloom) {
        // Exterior cottage
        draw_ground(cr, false);
        double hx = 720.0, hy = GY - 220.0;

        cairo_set_line_width(cr, 4.5);
        draw_rounded_rect(cr, hx - 140, hy, 280, 220, 12.0, true);
        cairo_move_to(cr, hx - 175, hy); cairo_line_to(cr, hx, hy - 120); cairo_line_to(cr, hx + 175, hy); cairo_close_path(cr); stroke_shape(cr, true);
        cairo_move_to(cr, hx + 85, hy - 50); cairo_line_to(cr, hx + 85, hy - 130);
        cairo_line_to(cr, hx + 115, hy - 130); cairo_line_to(cr, hx + 115, hy); cairo_stroke(cr);
        cairo_arc(cr, hx + 100, hy - 160, 18, 0, 2*M_PI); cairo_stroke(cr);
        draw_rounded_rect(cr, hx - 35, GY - 120, 70, 120, 6.0, true);
        draw_rounded_rect(cr, hx + 55, hy + 45, 60, 60, 6.0, true);
        cairo_move_to(cr, hx + 85, hy + 45); cairo_line_to(cr, hx + 85, hy + 105); cairo_stroke(cr);
        cairo_move_to(cr, hx + 55, hy + 75); cairo_line_to(cr, hx + 115, hy + 75); cairo_stroke(cr);

        draw_mit_standing(cr, 280, false);
    } else {
        // Interior: Warm dining table with family members sitting BEHIND table!
        draw_ground(cr, true);
        double tx = W / 2.0;

        // 1. Hanging ceiling lamp
        cairo_set_line_width(cr, 4.0);
        cairo_move_to(cr, tx, 40); cairo_line_to(cr, tx, GY - 300); cairo_stroke(cr);
        cairo_arc(cr, tx, GY - 280, 48, M_PI, 2*M_PI); cairo_close_path(cr); stroke_shape(cr, true);
        draw_sun(cr, tx, GY - 240, 20);

        // 2. Family members bodies and heads (sitting behind table)
        // Mít (center left)
        draw_mit_head(cr, tx - 70, GY - 200, 52, true);

        // Mom (center right)
        double mom_x = tx + 70, mom_y = GY - 200;
        cairo_arc(cr, mom_x + 48, mom_y - 10, 18, 0, 2*M_PI); stroke_shape(cr, true); // Ponytail
        cairo_arc(cr, mom_x, mom_y, 52, 0, 2*M_PI); stroke_shape(cr, true);
        cairo_arc(cr, mom_x - 14, mom_y - 6, 5, 0, 2*M_PI); cairo_arc(cr, mom_x + 14, mom_y - 6, 5, 0, 2*M_PI); cairo_fill(cr);
        cairo_arc(cr, mom_x, mom_y + 16, 18, 0.2*M_PI, 0.8*M_PI); cairo_stroke(cr);

        // Grandma (far right)
        double gm_x = tx + 200, gm_y = GY - 200;
        cairo_arc(cr, gm_x + 48, gm_y - 10, 20, 0, 2*M_PI); stroke_shape(cr, true); // Bun
        cairo_arc(cr, gm_x, gm_y, 52, 0, 2*M_PI); stroke_shape(cr, true);
        cairo_arc(cr, gm_x - 14, gm_y - 6, 11, 0, 2*M_PI); cairo_stroke(cr);
        cairo_arc(cr, gm_x + 14, gm_y - 6, 11, 0, 2*M_PI); cairo_stroke(cr);

        // Dad (far left)
        double d_x = tx - 200, d_y = GY - 200;
        cairo_arc(cr, d_x, d_y, 52, 0, 2*M_PI); stroke_shape(cr, true);
        cairo_arc(cr, d_x - 14, d_y - 6, 5, 0, 2*M_PI); cairo_arc(cr, d_x + 14, d_y - 6, 5, 0, 2*M_PI); cairo_fill(cr);
        cairo_arc(cr, d_x, d_y + 16, 18, 0.2*M_PI, 0.8*M_PI); cairo_stroke(cr);

        // 3. Table in front (solid white fill covers lower bodies cleanly!)
        cairo_set_line_width(cr, 4.5);
        draw_rounded_rect(cr, tx - 240, GY - 130, 480, 110, 24.0, true);
        for (double bx : {tx - 150, tx - 50, tx + 50, tx + 150}) {
            cairo_arc(cr, bx, GY - 95, 22, 0, M_PI); cairo_close_path(cr); stroke_shape(cr, true);
        }

        draw_heart(cr, tx - 110, GY - 280, 28);
        draw_heart(cr, tx + 110, GY - 280, 28);
    }
}

// Master dispatch function
void draw_ehon_illustration(const std::string& filename, int page, bool is_bloom) {
    cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, W, H);
    cairo_t* cr = cairo_create(surface);

    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_paint(cr);

    switch (page) {
        case 1:  render_scene_1(cr, is_bloom); break;
        case 2:  render_scene_2(cr, is_bloom); break;
        case 3:  render_scene_3(cr, is_bloom); break;
        case 4:  render_scene_4(cr, is_bloom); break;
        case 5:  render_scene_5(cr, is_bloom); break;
        case 6:  render_scene_6(cr, is_bloom); break;
        case 7:  render_scene_7(cr, is_bloom); break;
        case 8:  render_scene_8(cr, is_bloom); break;
        case 9:  render_scene_9(cr, is_bloom); break;
        case 10: render_scene_10(cr, is_bloom); break;
        case 11: render_scene_11(cr, is_bloom); break;
        case 12: render_scene_12(cr, is_bloom); break;
        case 13: render_scene_13(cr, is_bloom); break;
        case 14: render_scene_14(cr, is_bloom); break;
        case 15: render_scene_15(cr, is_bloom); break;
        case 16: render_scene_16(cr, is_bloom); break;
        default: render_scene_1(cr, is_bloom); break;
    }

    cairo_destroy(cr);
    cairo_surface_write_to_png(surface, filename.c_str());
    cairo_surface_destroy(surface);
}

int main() {
    std::cout << "Regenerating all 32 illustrations with solid opaque white craftsmanship..." << std::endl;
    for (int p = 1; p <= 16; ++p) {
        std::string dry_name = "assets/images/page" + std::to_string(p) + "_dry.png";
        std::string bloom_name = "assets/images/page" + std::to_string(p) + "_bloom.png";
        draw_ehon_illustration(dry_name, p, false);
        draw_ehon_illustration(bloom_name, p, true);
        std::cout << "  ✓ Page " << p << " done" << std::endl;
    }
    std::cout << "All 32 illustrations generated with zero-overlap perfection!" << std::endl;
    return 0;
}
