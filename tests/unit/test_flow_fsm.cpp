#include <cassert>
#include <iostream>
#include "domain/flow_fsm.hpp"

using namespace gach::domain;

void test_initial_state_is_observe() {
    FlowFSM fsm;
    assert(fsm.state() == BeatState::OBSERVE);
    assert(fsm.is_touch_locked() == true);
    assert(fsm.hint_visible() == false);
    std::cout << "  ✓ test_initial_state_is_observe passed" << std::endl;
}

void test_touch_blocked_in_observe() {
    FlowFSM fsm;
    // Bất kỳ thao tác chạm nào trong nhịp OBSERVE đều bị chặn (chống bấm loạn)
    bool handled = fsm.handle_touch_choice(0, true);
    assert(handled == false);
    assert(fsm.state() == BeatState::OBSERVE);
    assert(fsm.is_touch_locked() == true);
    std::cout << "  ✓ test_touch_blocked_in_observe passed" << std::endl;
}

void test_transition_to_await_input() {
    FlowFSM fsm;
    bool ok = fsm.on_observe_timeout();
    assert(ok == true);
    assert(fsm.state() == BeatState::AWAIT_INPUT);
    assert(fsm.is_touch_locked() == false);
    std::cout << "  ✓ test_transition_to_await_input passed" << std::endl;
}

void test_incorrect_choice_keeps_await_input() {
    FlowFSM fsm;
    fsm.on_observe_timeout();

    // Chọn sai: không phạt, không trừ điểm, FSM vẫn ở AWAIT_INPUT
    bool handled = fsm.handle_touch_choice(1, false);
    assert(handled == true);
    assert(fsm.state() == BeatState::AWAIT_INPUT);
    assert(fsm.incorrect_attempts() == 1);
    std::cout << "  ✓ test_incorrect_choice_keeps_await_input passed" << std::endl;
}

void test_hint_timeout_triggers_gentle_guide() {
    FlowFSM fsm;
    fsm.on_observe_timeout();
    assert(fsm.hint_visible() == false);

    // Sau 6 giây bé chưa chọn -> kích hoạt hint nhẹ nhàng
    fsm.on_hint_timeout();
    assert(fsm.hint_visible() == true);
    assert(fsm.state() == BeatState::AWAIT_INPUT);
    std::cout << "  ✓ test_hint_timeout_triggers_gentle_guide passed" << std::endl;
}

void test_correct_choice_transitions_to_transform() {
    FlowFSM fsm;
    fsm.on_observe_timeout();

    // Bé chạm đúng mảnh ghép
    bool handled = fsm.handle_touch_choice(0, true);
    assert(handled == true);
    assert(fsm.state() == BeatState::TRANSFORM);
    assert(fsm.is_touch_locked() == true); // Khoảng lặng chuyển hóa: khóa cảm ứng
    std::cout << "  ✓ test_correct_choice_transitions_to_transform passed" << std::endl;
}

void test_transform_to_completed() {
    FlowFSM fsm;
    fsm.on_observe_timeout();
    fsm.handle_touch_choice(0, true);

    // Khoảng lặng chuyển hóa kết thúc
    bool ok = fsm.on_transform_finished();
    assert(ok == true);
    assert(fsm.state() == BeatState::COMPLETED);
    assert(fsm.is_touch_locked() == false); // Mở nút sang trang tiếp theo
    std::cout << "  ✓ test_transform_to_completed passed" << std::endl;
}

void test_strict_invalid_transitions() {
    FlowFSM fsm;
    // Cấm nhảy từ OBSERVE sang TRANSFORM
    assert(fsm.on_transform_finished() == false);
    assert(fsm.state() == BeatState::OBSERVE);

    // Cấm gọi hint khi đang ở OBSERVE
    fsm.on_hint_timeout();
    assert(fsm.hint_visible() == false);

    // Chuyển sang COMPLETED rồi reset về trang mới
    fsm.on_observe_timeout();
    fsm.handle_touch_choice(0, true);
    fsm.on_transform_finished();
    assert(fsm.state() == BeatState::COMPLETED);

    // Reset sang trang mới
    fsm.reset_for_new_page();
    assert(fsm.state() == BeatState::OBSERVE);
    assert(fsm.is_touch_locked() == true);
    assert(fsm.hint_visible() == false);
    assert(fsm.incorrect_attempts() == 0);
    std::cout << "  ✓ test_strict_invalid_transitions passed" << std::endl;
}

int main() {
    std::cout << "--- Running FlowFSM Unit Tests ---" << std::endl;
    test_initial_state_is_observe();
    test_touch_blocked_in_observe();
    test_transition_to_await_input();
    test_incorrect_choice_keeps_await_input();
    test_hint_timeout_triggers_gentle_guide();
    test_correct_choice_transitions_to_transform();
    test_transform_to_completed();
    test_strict_invalid_transitions();
    std::cout << "All FlowFSM tests passed!" << std::endl;
    return 0;
}
