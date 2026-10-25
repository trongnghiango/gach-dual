#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "adapters/ui/ui_layout.hpp"
#include "domain/story_beat.hpp"
#include "domain/flow_fsm.hpp"

namespace gach::application::ports {

/**
 * @brief Outbound Port trừu tượng hóa việc vẽ giao diện lên màn hình E-Ink.
 */
class IDisplayRenderer {
public:
    virtual ~IDisplayRenderer() = default;

    virtual void render_beat(const domain::StoryBeat& beat,
                            const domain::FlowFSM& fsm,
                            bool full_refresh) = 0;

    virtual void request_redraw(bool full_refresh) = 0;
    virtual void set_resolution(int width, int height) = 0;

    virtual void set_overlay_state([[maybe_unused]] adapters::ui::OverlayMode mode,
                                   [[maybe_unused]] bool auto_hint,
                                   [[maybe_unused]] const std::vector<std::string>& words) {}
};

} // namespace gach::application::ports