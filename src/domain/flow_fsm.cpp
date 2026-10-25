#include "domain/flow_fsm.hpp"

namespace gach::domain {

FlowFSM::FlowFSM() noexcept
    : m_state(BeatState::OBSERVE),
      m_touch_locked(true),
      m_hint_visible(false),
      m_incorrect_attempts(0) {}

BeatState FlowFSM::state() const noexcept {
    return m_state;
}

bool FlowFSM::is_touch_locked() const noexcept {
    return m_touch_locked;
}

bool FlowFSM::hint_visible() const noexcept {
    return m_hint_visible;
}

uint32_t FlowFSM::incorrect_attempts() const noexcept {
    return m_incorrect_attempts;
}

bool FlowFSM::on_observe_timeout() noexcept {
    if (m_state != BeatState::OBSERVE) {
        return false;
    }
    m_state = BeatState::AWAIT_INPUT;
    m_touch_locked = false;
    return true;
}

bool FlowFSM::handle_touch_choice([[maybe_unused]] uint32_t choice_index, bool is_correct) noexcept {
    if (m_state != BeatState::AWAIT_INPUT || m_touch_locked) {
        return false;
    }

    if (is_correct) {
        m_state = BeatState::TRANSFORM;
        m_touch_locked = true; // Khóa cảm ứng trong khoảng lặng chuyển hóa
        m_hint_visible = false;
    } else {
        // Anti-Dopamine: Không phạt, chỉ ghi nhận và giữ nguyên trạng thái
        m_incorrect_attempts++;
    }
    return true;
}

void FlowFSM::on_hint_timeout() noexcept {
    if (m_state == BeatState::AWAIT_INPUT) {
        m_hint_visible = true;
    }
}

bool FlowFSM::on_transform_finished() noexcept {
    if (m_state != BeatState::TRANSFORM) {
        return false;
    }
    m_state = BeatState::COMPLETED;
    m_touch_locked = false; // Mở khóa để bé bấm nút sang trang
    return true;
}

void FlowFSM::reset_for_new_page() noexcept {
    m_state = BeatState::OBSERVE;
    m_touch_locked = true;
    m_hint_visible = false;
    m_incorrect_attempts = 0;
}

} // namespace gach::domain