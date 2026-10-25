#pragma once

#include <cstdint>
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
};

} // namespace gach::application::ports