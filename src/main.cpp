#include <fstream>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include "infrastructure/app_config.hpp"
#include "infrastructure/kindle_power.hpp"
#include "adapters/db/sqlite_story_repository.hpp"
#include "adapters/ui/ui_layout.hpp"
#include "adapters/ui/cairo_renderer.hpp"
#include "adapters/ui/gtk_touch_adapter.hpp"
#include "application/story_interactor.hpp"

using namespace gach;

static std::string read_text_file(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        const char* home = std::getenv("GACH_HOME");
        if (home) {
            std::string alt = std::string(home) + "/" + path;
            std::ifstream alt_file(alt);
            if (alt_file.is_open()) {
                std::stringstream buffer;
                buffer << alt_file.rdbuf();
                return buffer.str();
            }
        }
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main(int argc, char** argv) {
    std::cout << "================================================" << std::endl;
    std::cout << "  GACH Engine - Kindle-Mít Story Phonics (v1.0)  " << std::endl;
    std::cout << "  Kindle Paperwhite 2/3 E-Ink Optimized          " << std::endl;
    std::cout << "================================================" << std::endl;

    auto config = infrastructure::AppConfig::parse_args(argc, argv);
    infrastructure::KindlePower::init_power_guard();

    // 1. Khởi tạo Cơ sở dữ liệu SQLite
    adapters::db::SqliteStoryRepository repo(config.db_path);
    if (!repo.initialize()) {
        std::cerr << "[ERROR] Failed to initialize SQLite database at: " << config.db_path << std::endl;
        return 1;
    }

    // Tự động kiểm tra và seed dữ liệu nếu DB mới
    if (!repo.load_story(config.story_id)) {
        std::cout << "[INFO] Database empty or story not found. Seeding initial story..." << std::endl;
        std::string schema = read_text_file("data/schema.sql");
        std::string seed = read_text_file("data/seed_story_mit.sql");
        std::string seed16 = read_text_file("data/seed_story_mit_16pages.sql");
        if (!schema.empty()) {
            repo.execute_sql(schema);
            if (!seed.empty()) repo.execute_sql(seed);
            if (!seed16.empty()) repo.execute_sql(seed16);
        }
    }

    // 2. Thiết lập Bố cục và Renderer Cairo
    adapters::ui::UILayout layout;
    adapters::ui::CairoRenderer renderer(layout);

    // 3. Khởi tạo Use Case Interactor
    application::StoryInteractor interactor(repo, renderer);
    if (!interactor.start_story(config.story_id)) {
        std::cout << "[WARN] Story '" << config.story_id << "' not found. Searching for available fallback stories..." << std::endl;
        if (interactor.start_story("mit_ngay_tu_te")) {
            config.story_id = "mit_ngay_tu_te";
            std::cout << "[INFO] Successfully loaded story: mit_ngay_tu_te" << std::endl;
        } else if (interactor.start_story("mit_tuoi_cay")) {
            config.story_id = "mit_tuoi_cay";
            std::cout << "[INFO] Successfully loaded story: mit_tuoi_cay" << std::endl;
        } else {
            std::cerr << "[ERROR] No valid story found in database at: " << config.db_path << std::endl;
            return 1;
        }
    }

    std::cout << "[INFO] Loaded story: '" << repo.story_title() << "' with " << repo.total_pages() << " pages." << std::endl;
    std::cout << "[INFO] Window Resolution: " << config.width << "x" << config.height << std::endl;

    // 4. Khởi chạy GTK Touch Adapter
    adapters::ui::GtkTouchAdapter gtk_adapter(interactor, renderer);
    gtk_adapter.initialize(argc, argv, config.width, config.height);
    gtk_adapter.run();

    infrastructure::KindlePower::restore_power();
    std::cout << "[INFO] GACH Engine closed cleanly." << std::endl;
    return 0;
}