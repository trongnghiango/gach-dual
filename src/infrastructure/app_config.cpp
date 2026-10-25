#include "infrastructure/app_config.hpp"
#include <cstring>
#include <unistd.h>

namespace gach::infrastructure {

AppConfig AppConfig::parse_args(int argc, char** argv) {
    AppConfig config;

    // Tự động phát hiện nếu đang chạy trên Kindle (Kiểm tra lipc-set-prop bằng access(F_OK))
    config.is_kindle = (access("/usr/bin/lipc-set-prop", F_OK) == 0);

    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--db") == 0 && i + 1 < argc) {
            config.db_path = argv[++i];
        } else if (std::strcmp(argv[i], "--story") == 0 && i + 1 < argc) {
            config.story_id = argv[++i];
        } else if (std::strcmp(argv[i], "--pw2") == 0) {
            config.width = 758;
            config.height = 1024;
        } else if (std::strcmp(argv[i], "--pw3") == 0) {
            config.width = 1072;
            config.height = 1448;
        } else if (std::strcmp(argv[i], "--desktop") == 0) {
            config.width = 600;
            config.height = 850;
        }
    }
    return config;
}

} // namespace gach::infrastructure