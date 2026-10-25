#pragma once

namespace gach::infrastructure {

class KindlePower {
public:
    static void init_power_guard();
    static void prevent_sleep();
    static void restore_power();
};

} // namespace gach::infrastructure