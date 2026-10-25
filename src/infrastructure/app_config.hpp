#pragma once

#include <string>

namespace gach::infrastructure {

struct AppConfig {
    std::string db_path{"story_engine.db"};
    std::string story_id{"mit_tuoi_cay"};
    int width{1072};
    int height{1448};
    bool is_kindle{false};

    static AppConfig parse_args(int argc, char** argv);
};

} // namespace gach::infrastructure