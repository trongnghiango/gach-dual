#include "adapters/ui/ink_flex.hpp"
#include <numeric>

namespace gach::adapters::ui {

void FlexNode::layout(int parent_x, int parent_y, int parent_w, int parent_h) {
    // 1. XÁC ĐỊNH TỌA ĐỘ VÀ KÍCH THƯỚC CỦA CHÍNH MÌNH
    if (position == PositionType::Absolute) {
        // Chiều rộng
        if (left != AUTO && right != AUTO) {
            computed_w = parent_w - left - right - margin.left - margin.right;
        } else if (width != AUTO) {
            computed_w = width;
        } else if (measure_func) {
            computed_w = measure_func(parent_w).first;
        } else {
            computed_w = parent_w;
        }

        // Chiều cao
        if (top != AUTO && bottom != AUTO) {
            computed_h = parent_h - top - bottom - margin.top - margin.bottom;
        } else if (height != AUTO) {
            computed_h = height;
        } else if (measure_func) {
            computed_h = measure_func(computed_w).second;
        } else {
            computed_h = parent_h;
        }

        computed_w = std::clamp(computed_w, min_width, max_width);
        computed_h = std::clamp(computed_h, min_height, max_height);

        // Tọa độ X
        if (left != AUTO) {
            computed_x = parent_x + left + margin.left;
        } else if (right != AUTO) {
            computed_x = parent_x + parent_w - right - margin.right - computed_w;
        } else {
            computed_x = parent_x + margin.left;
        }

        // Tọa độ Y
        if (top != AUTO) {
            computed_y = parent_y + top + margin.top;
        } else if (bottom != AUTO) {
            computed_y = parent_y + parent_h - bottom - margin.bottom - computed_h;
        } else {
            computed_y = parent_y + margin.top;
        }
    } else {
        // Position Relative: Đã được cha chỉ định kích thước và tọa độ
        computed_x = parent_x + margin.left;
        computed_y = parent_y + margin.top;
        computed_w = (width != AUTO) ? width : parent_w;
        computed_h = (height != AUTO) ? height : parent_h;

        if (measure_func && height == AUTO) {
            computed_h = measure_func(computed_w).second;
        }

        computed_w = std::clamp(computed_w, min_width, max_width);
        computed_h = std::clamp(computed_h, min_height, max_height);
    }

    // 2. TÍNH TOÁN VÙNG NỘI DUNG BÊN TRONG (INNER CONTENT BOUNDS)
    int inner_x = computed_x + padding.left;
    int inner_y = computed_y + padding.top;
    int inner_w = std::max(0, computed_w - padding.left - padding.right);
    int inner_h = std::max(0, computed_h - padding.top - padding.bottom);

    if (children.empty()) {
        return;
    }

    // 3. TÁCH BIỆT CÁC CON RELATIVE VÀ ABSOLUTE
    std::vector<std::shared_ptr<FlexNode>> relative_children;
    std::vector<std::shared_ptr<FlexNode>> absolute_children;

    for (const auto& child : children) {
        if (!child) continue;
        if (child->position == PositionType::Absolute) {
            absolute_children.push_back(child);
        } else {
            relative_children.push_back(child);
        }
    }

    // 4. BỐ CỤC DÒNG CHẢY FLEXBOX (CHO CÁC CON RELATIVE)
    if (!relative_children.empty()) {
        int rel_count = static_cast<int>(relative_children.size());

        if (direction == FlexDirection::Column) {
            // TRỤC CHÍNH: DỌC (Y), TRỤC PHỤ: NGANG (X)
            int total_fixed_h = 0;
            float total_flex_grow = 0.0f;

            // Đo kích thước ban đầu của từng con
            for (auto& child : relative_children) {
                int child_w = (child->width != AUTO) ? child->width : inner_w;
                int child_h = (child->height != AUTO) ? child->height : 0;

                if (child->measure_func) {
                    auto [mw, mh] = child->measure_func(child_w);
                    if (child->height == AUTO) child_h = mh;
                }

                child->computed_w = child_w;
                child->computed_h = child_h;

                if (child->flex_grow > 0.0f) {
                    total_flex_grow += child->flex_grow;
                } else {
                    total_fixed_h += child_h + child->margin.top + child->margin.bottom;
                }
            }

            int total_gaps = (rel_count - 1) * gap;
            int free_space = inner_h - total_fixed_h - total_gaps;

            // Phân bổ phần không gian dư thừa cho các phần tử có flex_grow
            if (total_flex_grow > 0.0f && free_space > 0) {
                for (auto& child : relative_children) {
                    if (child->flex_grow > 0.0f) {
                        int grow_h = static_cast<int>(free_space * (child->flex_grow / total_flex_grow));
                        child->computed_h = std::clamp(grow_h, child->min_height, child->max_height);
                    }
                }
            }

            // Định vị tọa độ tuần tự không chồng lấn
            int cur_y = inner_y;
            for (auto& child : relative_children) {
                child->computed_y = cur_y + child->margin.top;

                // Căn chỉnh trục phụ (AlignItems)
                if (align == AlignItems::Center) {
                    child->computed_x = inner_x + (inner_w - child->computed_w) / 2;
                } else {
                    child->computed_x = inner_x + child->margin.left;
                }

                // Gọi đệ quy layout cho các con của con
                child->layout(child->computed_x, child->computed_y, child->computed_w, child->computed_h);

                cur_y += child->computed_h + child->margin.top + child->margin.bottom + gap;
            }

        } else {
            // TRỤC CHÍNH: NGANG (X), TRỤC PHỤ: DỌC (Y)
            int total_fixed_w = 0;
            float total_flex_grow = 0.0f;

            for (auto& child : relative_children) {
                int child_w = (child->width != AUTO) ? child->width : 0;
                int child_h = (child->height != AUTO) ? child->height : inner_h;

                if (child->measure_func) {
                    auto [mw, mh] = child->measure_func(inner_w);
                    if (child->width == AUTO) child_w = mw;
                }

                child->computed_w = child_w;
                child->computed_h = child_h;

                if (child->flex_grow > 0.0f) {
                    total_flex_grow += child->flex_grow;
                } else {
                    total_fixed_w += child_w + child->margin.left + child->margin.right;
                }
            }

            int total_gaps = (rel_count - 1) * gap;
            int free_space = inner_w - total_fixed_w - total_gaps;

            if (total_flex_grow > 0.0f && free_space > 0) {
                for (auto& child : relative_children) {
                    if (child->flex_grow > 0.0f) {
                        int grow_w = static_cast<int>(free_space * (child->flex_grow / total_flex_grow));
                        child->computed_w = std::clamp(grow_w, child->min_width, child->max_width);
                    }
                }
            }

            int cur_x = inner_x;
            if (justify == JustifyContent::SpaceBetween && rel_count > 1 && total_flex_grow == 0.0f && free_space > 0) {
                int space_gap = free_space / (rel_count - 1);
                for (auto& child : relative_children) {
                    child->computed_x = cur_x + child->margin.left;
                    if (align == AlignItems::Center) {
                        child->computed_y = inner_y + (inner_h - child->computed_h) / 2;
                    } else {
                        child->computed_y = inner_y + child->margin.top;
                    }
                    child->layout(child->computed_x, child->computed_y, child->computed_w, child->computed_h);
                    cur_x += child->computed_w + child->margin.left + child->margin.right + space_gap;
                }
            } else {
                for (auto& child : relative_children) {
                    child->computed_x = cur_x + child->margin.left;
                    if (align == AlignItems::Center) {
                        child->computed_y = inner_y + (inner_h - child->computed_h) / 2;
                    } else {
                        child->computed_y = inner_y + child->margin.top;
                    }
                    child->layout(child->computed_x, child->computed_y, child->computed_w, child->computed_h);
                    cur_x += child->computed_w + child->margin.left + child->margin.right + gap;
                }
            }
        }
    }

    // 5. BỐ CỤC CÁC PHẦN TỬ ABSOLUTE
    for (auto& abs_child : absolute_children) {
        abs_child->layout(computed_x, computed_y, computed_w, computed_h);
    }
}

void FlexNode::collect_render_list(std::vector<FlexNode*>& out_list) {
    out_list.push_back(this);
    for (auto& child : children) {
        if (child) {
            child->collect_render_list(out_list);
        }
    }
}

FlexNode* FlexNode::hit_test(int px, int py) {
    std::vector<FlexNode*> list;
    collect_render_list(list);

    // Đảo ngược danh sách để các phần tử con / vẽ sau được ưu tiên nhận sự kiện trước phần tử cha
    std::reverse(list.begin(), list.end());

    // Sắp xếp theo z-index giảm dần (Painter's Hit-Testing)
    std::stable_sort(list.begin(), list.end(), [](const FlexNode* a, const FlexNode* b) {
        return a->z_index > b->z_index;
    });

    for (auto* node : list) {
        if (!node->id.empty() && node->contains(px, py)) {
            return node;
        }
    }
    return nullptr;
}

} // namespace gach::adapters::ui
