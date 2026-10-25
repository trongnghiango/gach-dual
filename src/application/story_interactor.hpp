#pragma once

#include <cstdint>
#include <optional>
#include <string_view>
#include <vector>
#include "application/ports/display_renderer_port.hpp"
#include "application/ports/story_repository_port.hpp"
#include "application/ports/user_input_port.hpp"
#include "adapters/ui/ui_layout.hpp"
#include "domain/flow_fsm.hpp"
#include "domain/story_beat.hpp"

namespace gach::application {

using OverlayMode = adapters::ui::OverlayMode;

class StoryInteractor : public ports::IUserInputPort {
public:
    StoryInteractor(ports::IStoryRepository& repo, ports::IDisplayRenderer& renderer);

    bool start_story(std::string_view story_id);
    void on_touch(int x, int y) override;
    void on_timer_tick(uint32_t delta_ms) override;

    // Điều hướng trang
    bool advance_page();
    bool go_previous_page();
    bool jump_to_page(uint32_t page);

    // Điều khiển Modal Overlays
    [[nodiscard]] OverlayMode overlay_mode() const noexcept { return m_overlay_mode; }
    void open_menu() noexcept;
    void open_settings() noexcept;
    void close_overlay() noexcept;

    // Cài đặt
    [[nodiscard]] bool auto_hint_enabled() const noexcept { return m_auto_hint; }
    void toggle_auto_hint() noexcept;
    void reset_story() noexcept;

    [[nodiscard]] const domain::FlowFSM& fsm() const noexcept { return m_fsm; }
    [[nodiscard]] uint32_t current_page() const noexcept { return m_current_page; }
    [[nodiscard]] const std::optional<domain::StoryBeat>& current_beat() const noexcept { return m_current_beat; }
    [[nodiscard]] adapters::ui::UILayout& layout() noexcept { return m_layout; }
    [[nodiscard]] ports::IStoryRepository& repo() noexcept { return m_repo; }

    // Danh sách 16 từ ngữ mục tiêu cho Mục Lục
    [[nodiscard]] std::vector<std::string> get_all_story_words();

private:
    ports::IStoryRepository& m_repo;
    ports::IDisplayRenderer& m_renderer;
    adapters::ui::UILayout m_layout{};
    domain::FlowFSM m_fsm{};
    uint32_t m_current_page{1};
    std::optional<domain::StoryBeat> m_current_beat{std::nullopt};
    uint32_t m_timer_ms{0};
    uint32_t m_last_touch_time{0};

    OverlayMode m_overlay_mode{OverlayMode::NONE};
    bool m_auto_hint{true};
    std::string m_active_story_id{"mit_ngay_tu_te"};
};

} // namespace gach::application
