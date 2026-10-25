#include "application/story_interactor.hpp"
#include <iostream>

namespace gach::application {

StoryInteractor::StoryInteractor(ports::IStoryRepository& repo, ports::IDisplayRenderer& renderer)
    : m_repo(repo), m_renderer(renderer) {
    m_layout.update_resolution(1072, 1448);
}

bool StoryInteractor::start_story(std::string_view story_id) {
    if (!m_repo.load_story(story_id)) {
        return false;
    }
    m_current_page = 1;
    m_current_beat = m_repo.get_beat(m_current_page);
    if (!m_current_beat.has_value()) {
        return false;
    }

    m_fsm.reset_for_new_page();
    m_timer_ms = 0;

    // Yêu cầu vẽ trang đầu tiên ở trạng thái OBSERVE (Full Refresh)
    m_renderer.render_beat(*m_current_beat, m_fsm, true);
    return true;
}

void StoryInteractor::on_timer_tick(uint32_t delta_ms) {
    if (!m_current_beat.has_value()) return;

    m_timer_ms += delta_ms;

    switch (m_fsm.state()) {
        case domain::BeatState::OBSERVE:
            // 2.5s quan sát tĩnh kết thúc -> Chuyển sang AWAIT_INPUT
            if (m_timer_ms >= 2500) {
                if (m_fsm.on_observe_timeout()) {
                    m_timer_ms = 0;
                    m_renderer.render_beat(*m_current_beat, m_fsm, false); // Partial refresh
                }
            }
            break;

        case domain::BeatState::AWAIT_INPUT:
            // 6.0s không tương tác -> Dẫn lối nhẹ nhàng bằng viền nổi bật
            if (m_timer_ms >= 6000 && !m_fsm.hint_visible()) {
                m_fsm.on_hint_timeout();
                m_renderer.render_beat(*m_current_beat, m_fsm, false); // Partial refresh
            }
            break;

        case domain::BeatState::TRANSFORM:
            // 2.0s khoảng lặng chuyển hóa quả ngọt kết thúc -> Hiện mũi tên chuyển trang
            if (m_timer_ms >= 2000) {
                if (m_fsm.on_transform_finished()) {
                    m_timer_ms = 0;
                    m_renderer.render_beat(*m_current_beat, m_fsm, false); // Partial refresh
                }
            }
            break;

        case domain::BeatState::COMPLETED:
            // Đã mở khóa nút Next, đợi bé chủ động chạm
            break;
    }
}

void StoryInteractor::on_touch(int x, int y) {
    if (!m_current_beat.has_value() || m_fsm.is_touch_locked()) {
        return;
    }

    if (m_fsm.state() == domain::BeatState::AWAIT_INPUT) {
        int choice_idx = m_layout.hit_test_choice(x, y, m_current_beat->choice_count());
        if (choice_idx >= 0 && choice_idx < m_current_beat->choice_count()) {
            bool is_correct = m_current_beat->choices()[static_cast<size_t>(choice_idx)].is_correct;
            m_fsm.handle_touch_choice(static_cast<uint32_t>(choice_idx), is_correct);

            if (is_correct) {
                m_timer_ms = 0;
                // Đổi sang tranh quả ngọt: Full Refresh để xóa ghosting và hiển thị tuyệt đẹp
                m_renderer.render_beat(*m_current_beat, m_fsm, true);
            } else {
                // Chọn chưa đúng: Vẽ lại nhẹ nhàng, không đổi trang, không phạt
                m_renderer.render_beat(*m_current_beat, m_fsm, false);
            }
        }
    } else if (m_fsm.state() == domain::BeatState::COMPLETED) {
        if (m_layout.hit_test_next(x, y)) {
            advance_page();
        }
    }
}

bool StoryInteractor::advance_page() {
    if (m_current_page < m_repo.total_pages()) {
        m_current_page++;
        m_current_beat = m_repo.get_beat(m_current_page);
        m_fsm.reset_for_new_page();
        m_timer_ms = 0;
        // Sang trang mới: Full Refresh
        if (m_current_beat.has_value()) {
            m_renderer.render_beat(*m_current_beat, m_fsm, true);
        }
        return true;
    }
    return false;
}

} // namespace gach::application