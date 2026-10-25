#pragma once

#include <cstdint>
#include <optional>
#include <string_view>
#include "application/ports/display_renderer_port.hpp"
#include "application/ports/story_repository_port.hpp"
#include "application/ports/user_input_port.hpp"
#include "adapters/ui/ui_layout.hpp"
#include "domain/flow_fsm.hpp"
#include "domain/story_beat.hpp"

namespace gach::application {

class StoryInteractor : public ports::IUserInputPort {
public:
    StoryInteractor(ports::IStoryRepository& repo, ports::IDisplayRenderer& renderer);

    bool start_story(std::string_view story_id);
    void on_touch(int x, int y) override;
    void on_timer_tick(uint32_t delta_ms) override;

    bool advance_page();

    [[nodiscard]] const domain::FlowFSM& fsm() const noexcept { return m_fsm; }
    [[nodiscard]] uint32_t current_page() const noexcept { return m_current_page; }
    [[nodiscard]] const std::optional<domain::StoryBeat>& current_beat() const noexcept { return m_current_beat; }
    [[nodiscard]] adapters::ui::UILayout& layout() noexcept { return m_layout; }

private:
    ports::IStoryRepository& m_repo;
    ports::IDisplayRenderer& m_renderer;
    adapters::ui::UILayout m_layout{};
    domain::FlowFSM m_fsm{};
    uint32_t m_current_page{1};
    std::optional<domain::StoryBeat> m_current_beat{std::nullopt};
    uint32_t m_timer_ms{0};
    uint32_t m_last_touch_time{0};
};

} // namespace gach::application