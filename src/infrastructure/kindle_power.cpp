#include "infrastructure/kindle_power.hpp"
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <unistd.h>

namespace gach::infrastructure {

static void handle_exit_signal([[maybe_unused]] int sig) {
    KindlePower::restore_power();
    std::exit(0);
}

void KindlePower::init_power_guard() {
    std::signal(SIGINT, handle_exit_signal);
    std::signal(SIGTERM, handle_exit_signal);
}

void KindlePower::prevent_sleep() {
    // Kiểm tra file lipc-set-prop bằng access(F_OK) trên FAT32/Rootfs
    if (access("/usr/bin/lipc-set-prop", F_OK) == 0) {
        // Giả lập sự kiện chạm để giữ màn hình sáng
        [[maybe_unused]] int rc = std::system("/usr/bin/lipc-set-prop com.lab126.powerd -i touchScreenEvent 1 2>/dev/null");
    }
}

void KindlePower::restore_power() {
    // Khôi phục trạng thái nguồn khi đóng ứng dụng
}

} // namespace gach::infrastructure