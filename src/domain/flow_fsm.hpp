#pragma once

#include <cstdint>
#include "domain/domain_errors.hpp"

namespace gach::domain {

enum class BeatState : uint8_t {
    OBSERVE = 1,     // Nhịp 1: Đọc lời dẫn & ngắm tranh 2.5s (Khóa cảm ứng)
    AWAIT_INPUT = 2, // Nhịp 2: Hiện mảnh ghép vần khuyết, bé chạm tương tác
    TRANSFORM = 3,   // Nhịp 3: Từ ghép hoàn chỉnh, tranh quả ngọt, khoảng lặng
    COMPLETED = 4    // Nhịp 4: Nút mũi tên xuất hiện để bé chủ động sang trang
};

class FlowFSM {
public:
    FlowFSM() noexcept;

    [[nodiscard]] BeatState state() const noexcept;
    [[nodiscard]] bool is_touch_locked() const noexcept;
    [[nodiscard]] bool hint_visible() const noexcept;
    [[nodiscard]] uint32_t incorrect_attempts() const noexcept;

    // Sự kiện 1: Hết 2.5s quan sát tĩnh
    bool on_observe_timeout() noexcept;

    // Sự kiện 2: Bé chạm vào một lựa chọn mảnh ghép
    bool handle_touch_choice(uint32_t choice_index, bool is_correct) noexcept;

    // Sự kiện 3: Tự động kích hoạt hướng dẫn nhẹ nhàng sau 6.0s không tương tác
    void on_hint_timeout() noexcept;

    // Sự kiện 4: Khoảng lặng chuyển hóa kết thúc (chuyển sang nhịp Completed)
    bool on_transform_finished() noexcept;

    // Reset trạng thái để nạp trang truyện mới
    void reset_for_new_page() noexcept;

private:
    BeatState m_state{BeatState::OBSERVE};
    bool m_touch_locked{true};
    bool m_hint_visible{false};
    uint32_t m_incorrect_attempts{0};
};

} // namespace gach::domain