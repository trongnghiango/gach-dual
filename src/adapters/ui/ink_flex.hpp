#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace gach::adapters::ui {

struct Edges {
    int top{0};
    int right{0};
    int bottom{0};
    int left{0};

    static constexpr Edges all(int v) noexcept { return {v, v, v, v}; }
    static constexpr Edges symmetric(int v, int h) noexcept { return {v, h, v, h}; }
};

enum class PositionType : uint8_t {
    Relative = 0,
    Absolute = 1
};

enum class FlexDirection : uint8_t {
    Column = 0,
    Row = 1
};

enum class JustifyContent : uint8_t {
    FlexStart = 0,
    Center = 1,
    FlexEnd = 2,
    SpaceBetween = 3,
    SpaceAround = 4
};

enum class AlignItems : uint8_t {
    Stretch = 0,
    Center = 1,
    FlexStart = 2,
    FlexEnd = 3
};

class FlexNode {
public:
    static constexpr int AUTO = -1;

    std::string id{};
    PositionType position{PositionType::Relative};
    FlexDirection direction{FlexDirection::Column};
    JustifyContent justify{JustifyContent::FlexStart};
    AlignItems align{AlignItems::Stretch};

    float flex_grow{0.0f};
    int width{AUTO};
    int height{AUTO};
    int min_width{0};
    int max_width{100000};
    int min_height{0};
    int max_height{100000};

    int top{AUTO};
    int right{AUTO};
    int bottom{AUTO};
    int left{AUTO};

    Edges margin{};
    Edges padding{};
    int gap{0};
    int16_t z_index{0};

    // Callback tùy chọn để đo kích thước nội dung thực tế (như text ngắt dòng)
    std::function<std::pair<int, int>(int available_w)> measure_func{nullptr};

    // Tọa độ và kích thước kết quả sau khi tính toán
    int computed_x{0};
    int computed_y{0};
    int computed_w{0};
    int computed_h{0};

    std::vector<std::shared_ptr<FlexNode>> children{};

    FlexNode() = default;
    explicit FlexNode(std::string node_id) : id(std::move(node_id)) {}

    // Builder methods (Fluent API)
    FlexNode& set_id(std::string node_id) { id = std::move(node_id); return *this; }
    FlexNode& set_position(PositionType p) { position = p; return *this; }
    FlexNode& set_direction(FlexDirection d) { direction = d; return *this; }
    FlexNode& set_justify(JustifyContent j) { justify = j; return *this; }
    FlexNode& set_align(AlignItems a) { align = a; return *this; }
    FlexNode& set_flex_grow(float f) { flex_grow = f; return *this; }
    FlexNode& set_size(int w, int h) { width = w; height = h; return *this; }
    FlexNode& set_width(int w) { width = w; return *this; }
    FlexNode& set_height(int h) { height = h; return *this; }
    FlexNode& set_min_height(int h) { min_height = h; return *this; }
    FlexNode& set_max_height(int h) { max_height = h; return *this; }
    FlexNode& set_margin(Edges m) { margin = m; return *this; }
    FlexNode& set_padding(Edges p) { padding = p; return *this; }
    FlexNode& set_gap(int g) { gap = g; return *this; }
    FlexNode& set_z_index(int16_t z) { z_index = z; return *this; }

    FlexNode& set_top(int t) { top = t; return *this; }
    FlexNode& set_right(int r) { right = r; return *this; }
    FlexNode& set_bottom(int b) { bottom = b; return *this; }
    FlexNode& set_left(int l) { left = l; return *this; }

    FlexNode& add_child(std::shared_ptr<FlexNode> child) {
        if (child) children.push_back(std::move(child));
        return *this;
    }

    [[nodiscard]] bool contains(int px, int py) const noexcept {
        return px >= computed_x && px <= (computed_x + computed_w) &&
               py >= computed_y && py <= (computed_y + computed_h);
    }

    // Thuật toán bố cục đệ quy Flexbox & Absolute
    void layout(int parent_x, int parent_y, int parent_w, int parent_h);

    // Thu thập toàn bộ các Node theo thứ tự z-index để vẽ đúng lớp (Painter's Algorithm)
    void collect_render_list(std::vector<FlexNode*>& out_list);

    // Hit-testing theo thứ tự z-index từ cao xuống thấp (Topmost First)
    FlexNode* hit_test(int px, int py);
};

// Helper Factories
inline std::shared_ptr<FlexNode> make_column(std::string id = "") {
    auto n = std::make_shared<FlexNode>(std::move(id));
    n->direction = FlexDirection::Column;
    return n;
}

inline std::shared_ptr<FlexNode> make_row(std::string id = "") {
    auto n = std::make_shared<FlexNode>(std::move(id));
    n->direction = FlexDirection::Row;
    return n;
}

inline std::shared_ptr<FlexNode> make_item(std::string id = "") {
    return std::make_shared<FlexNode>(std::move(id));
}

} // namespace gach::adapters::ui
