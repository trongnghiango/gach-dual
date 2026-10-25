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
    m_active_story_id = std::string(story_id);
    m_current_page = 1;
    m_current_beat = m_repo.get_beat(m_current_page);
    if (!m_current_beat.has_value()) {
        return false;
    }

    m_fsm.reset_for_new_page();
    m_timer_ms = 0;
    m_overlay_mode = OverlayMode::NONE;

    m_renderer.set_overlay_state(m_overlay_mode, m_auto_hint, get_all_story_words());
    m_renderer.render_beat(*m_current_beat, m_fsm, true);
    return true;
}

void StoryInteractor::on_timer_tick(uint32_t delta_ms) {
    if (!m_current_beat.has_value()) return;

    // Khi đang mở Modal Mục Lục hoặc Cài Đặt, tạm dừng đồng hồ gợi ý
    if (m_overlay_mode != OverlayMode::NONE) {
        return;
    }

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
            // 6.0s không tương tác (nếu bật auto_hint) -> Dẫn lối nhẹ nhàng
            if (m_auto_hint && m_timer_ms >= 6000 && !m_fsm.hint_visible()) {
                m_fsm.on_hint_timeout();
                m_renderer.render_beat(*m_current_beat, m_fsm, false); // Partial refresh
            }
            break;

        case domain::BeatState::TRANSFORM:
            // 2.0s khoảng lặng chuyển hóa quả ngọt kết thúc -> Hiện nút chuyển trang
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
    if (!m_current_beat.has_value()) {
        return;
    }

    // 1. NẾU ĐANG MỞ MỤC LỤC OVERLAY
    if (m_overlay_mode == OverlayMode::MENU_TOC) {
        if (m_layout.hit_test_toc_close(x, y)) {
            close_overlay();
            return;
        }
        int target_page = m_layout.hit_test_toc_item(x, y);
        if (target_page >= 1 && target_page <= static_cast<int>(m_repo.total_pages())) {
            jump_to_page(static_cast<uint32_t>(target_page));
            close_overlay();
            return;
        }
        return;
    }

    // 2. NẾU ĐANG MỞ CÀI ĐẶT OVERLAY
    if (m_overlay_mode == OverlayMode::SETTINGS) {
        if (m_layout.hit_test_settings_close(x, y)) {
            close_overlay();
            return;
        }
        if (m_layout.hit_test_settings_toggle_hint(x, y)) {
            toggle_auto_hint();
            return;
        }
        if (m_layout.hit_test_settings_reset(x, y)) {
            reset_story();
            close_overlay();
            return;
        }
        return;
    }

    // 3. KIỂM TRA CÁC NÚT ĐIỀU HƯỚNG TRÊN ĐỈNH (TOP BAR)
    if (m_layout.hit_test_back(x, y)) {
        if (m_current_page > 1) {
            go_previous_page();
        }
        return;
    }

    if (m_layout.hit_test_menu(x, y)) {
        open_menu();
        return;
    }

    if (m_layout.hit_test_settings(x, y)) {
        open_settings();
        return;
    }

    // 4. KIỂM TRA TƯƠNG TÁC BÀI HỌC TRUYỆN
    if (m_fsm.is_touch_locked()) {
        return;
    }

    if (m_fsm.state() == domain::BeatState::AWAIT_INPUT) {
        int choice_idx = m_layout.hit_test_choice(x, y, m_current_beat->choice_count());
        if (choice_idx >= 0 && choice_idx < m_current_beat->choice_count()) {
            bool is_correct = m_current_beat->choices()[static_cast<size_t>(choice_idx)].is_correct;
            m_fsm.handle_touch_choice(static_cast<uint32_t>(choice_idx), is_correct);

            if (is_correct) {
                m_timer_ms = 0;
                // Đổi sang tranh quả ngọt: Full Refresh
                m_renderer.render_beat(*m_current_beat, m_fsm, true);
            } else {
                // Chọn chưa đúng: Vẽ lại nhẹ nhàng, không phạt
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
        return jump_to_page(m_current_page + 1);
    }
    return false;
}

bool StoryInteractor::go_previous_page() {
    if (m_current_page > 1) {
        return jump_to_page(m_current_page - 1);
    }
    return false;
}

bool StoryInteractor::jump_to_page(uint32_t page) {
    if (page >= 1 && page <= m_repo.total_pages()) {
        m_current_page = page;
        m_current_beat = m_repo.get_beat(m_current_page);
        m_fsm.reset_for_new_page();
        m_timer_ms = 0;
        if (m_current_beat.has_value()) {
            m_renderer.set_overlay_state(m_overlay_mode, m_auto_hint, get_all_story_words());
            m_renderer.render_beat(*m_current_beat, m_fsm, true);
        }
        return true;
    }
    return false;
}

void StoryInteractor::open_menu() noexcept {
    m_overlay_mode = OverlayMode::MENU_TOC;
    m_renderer.set_overlay_state(m_overlay_mode, m_auto_hint, get_all_story_words());
    m_renderer.request_redraw(false);
}

void StoryInteractor::open_settings() noexcept {
    m_overlay_mode = OverlayMode::SETTINGS;
    m_renderer.set_overlay_state(m_overlay_mode, m_auto_hint, get_all_story_words());
    m_renderer.request_redraw(false);
}

void StoryInteractor::close_overlay() noexcept {
    m_overlay_mode = OverlayMode::NONE;
    m_renderer.set_overlay_state(m_overlay_mode, m_auto_hint, get_all_story_words());
    m_renderer.request_redraw(false);
}

void StoryInteractor::toggle_auto_hint() noexcept {
    m_auto_hint = !m_auto_hint;
    m_renderer.set_overlay_state(m_overlay_mode, m_auto_hint, get_all_story_words());
    m_renderer.request_redraw(false);
}

void StoryInteractor::reset_story() noexcept {
    jump_to_page(1);
}

std::vector<std::string> StoryInteractor::get_all_story_words() {
    std::vector<std::string> words;
    uint32_t total = m_repo.total_pages();
    for (uint32_t p = 1; p <= total; ++p) {
        auto b = m_repo.get_beat(p);
        if (b.has_value()) {
            words.push_back(std::string(b->syllable().full_word()));
        } else {
            words.push_back("");
        }
    }
    return words;
}

} // namespace gach::application
