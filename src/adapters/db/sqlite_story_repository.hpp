#pragma once

#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <sqlite3.h>
#include "application/ports/story_repository_port.hpp"
#include "domain/story_beat.hpp"

namespace gach::adapters::db {

class SqliteStoryRepository : public application::ports::IStoryRepository {
public:
    explicit SqliteStoryRepository(std::string db_path);
    ~SqliteStoryRepository() override;

    // Disable copy to enforce single handle ownership (RAII)
    SqliteStoryRepository(const SqliteStoryRepository&) = delete;
    SqliteStoryRepository& operator=(const SqliteStoryRepository&) = delete;

    bool initialize();
    bool execute_sql(const std::string& sql);

    // IStoryRepository Implementation
    bool load_story(std::string_view story_id) override;
    [[nodiscard]] uint32_t total_pages() const noexcept override;
    [[nodiscard]] std::string_view current_story_id() const noexcept override;
    [[nodiscard]] std::string_view story_title() const noexcept override;
    [[nodiscard]] std::optional<domain::StoryBeat> get_beat(uint32_t page_number) override;

private:
    void close();

    std::string m_db_path;
    sqlite3* m_db{nullptr};
    std::string m_current_story_id{};
    std::string m_story_title{};
    uint32_t m_total_pages{0};
};

} // namespace gach::adapters::db