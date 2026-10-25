#pragma once

#include <cstdint>

namespace gach::application::ports {

/**
 * @brief Inbound Port tiếp nhận các sự kiện chạm cảm ứng và bộ đếm thời gian.
 */
class IUserInputPort {
public:
    virtual ~IUserInputPort() = default;

    virtual void on_touch(int x, int y) = 0;
    virtual void on_timer_tick(uint32_t delta_ms) = 0;
};

} // namespace gach::application::ports