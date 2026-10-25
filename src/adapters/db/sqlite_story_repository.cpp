#include "adapters/db/sqlite_story_repository.hpp"
#include <iostream>

namespace gach::adapters::db {

SqliteStoryRepository::SqliteStoryRepository(std::string db_path)
    : m_db_path(std::move(db_path)) {}

SqliteStoryRepository::~SqliteStoryRepository() {
    close();
}

void SqliteStoryRepository::close() {
    if (m_db) {
        sqlite3_close_v2(m_db);
        m_db = nullptr;
    }
}

bool SqliteStoryRepository::initialize() {
    close();
    int rc = sqlite3_open_v2(m_db_path.c_str(), &m_db,
                             SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                             nullptr);
    if (rc != SQLITE_OK) {
        return false;
    }

    // Cấu hình bắt buộc theo tiêu chuẩn Embedded Kindle (RAM < 12MB & FAT32)
    const char* pragmas =
        "PRAGMA foreign_keys = ON; "
        "PRAGMA cache_size = -500; "
        "PRAGMA journal_mode = DELETE; "
        "PRAGMA synchronous = NORMAL; "
        "PRAGMA temp_store = MEMORY; ";

    char* err_msg = nullptr;
    rc = sqlite3_exec(m_db, pragmas, nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK) {
        if (err_msg) sqlite3_free(err_msg);
        return false;
    }
    return true;
}

bool SqliteStoryRepository::execute_sql(const std::string& sql) {
    if (!m_db) return false;
    char* err_msg = nullptr;
    int rc = sqlite3_exec(m_db, sql.c_str(), nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK) {
        if (err_msg) sqlite3_free(err_msg);
        return false;
    }
    return true;
}

bool SqliteStoryRepository::load_story(std::string_view story_id) {
    if (!m_db) return false;

    const char* query = "SELECT title, total_pages FROM stories WHERE id = ? LIMIT 1;";
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(m_db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return false;

    sqlite3_bind_text(stmt, 1, story_id.data(), static_cast<int>(story_id.size()), SQLITE_STATIC);

    bool found = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* title = sqlite3_column_text(stmt, 0);
        int pages = sqlite3_column_int(stmt, 1);

        m_current_story_id = std::string(story_id);
        m_story_title = title ? reinterpret_cast<const char*>(title) : "";
        m_total_pages = static_cast<uint32_t>(pages);
        found = true;
    }
    sqlite3_finalize(stmt);
    return found;
}

uint32_t SqliteStoryRepository::total_pages() const noexcept {
    return m_total_pages;
}

std::string_view SqliteStoryRepository::current_story_id() const noexcept {
    return m_current_story_id;
}

std::string_view SqliteStoryRepository::story_title() const noexcept {
    return m_story_title;
}

std::optional<domain::StoryBeat> SqliteStoryRepository::get_beat(uint32_t page_number) {
    if (!m_db || m_current_story_id.empty() || page_number == 0 || page_number > m_total_pages) {
        return std::nullopt;
    }

    const char* query =
        "SELECT p.lead_text, p.initial_image_path, p.transform_image_path, "
        "       t.id, t.full_word, t.onset, t.rhyme, t.tone, t.missing_part "
        "FROM pages p "
        "JOIN phonics_targets t ON p.story_id = t.story_id AND p.page_number = t.page_number "
        "WHERE p.story_id = ? AND p.page_number = ? LIMIT 1;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(m_db, query, -1, &stmt, nullptr) != SQLITE_OK) {
        return std::nullopt;
    }

    sqlite3_bind_text(stmt, 1, m_current_story_id.c_str(), static_cast<int>(m_current_story_id.size()), SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, static_cast<int>(page_number));

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return std::nullopt;
    }

    std::string lead_text = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    std::string init_img  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    std::string trans_img = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    int target_id         = sqlite3_column_int(stmt, 3);
    std::string full_word = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
    std::string onset     = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
    std::string rhyme     = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
    int tone_val          = sqlite3_column_int(stmt, 7);
    std::string miss_part = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8));

    sqlite3_finalize(stmt);

    // Query choices
    const char* choices_query = "SELECT display_text, is_correct FROM choices WHERE target_id = ? ORDER BY id ASC LIMIT 4;";
    sqlite3_stmt* cstmt = nullptr;
    if (sqlite3_prepare_v2(m_db, choices_query, -1, &cstmt, nullptr) != SQLITE_OK) {
        return std::nullopt;
    }
    sqlite3_bind_int(cstmt, 1, target_id);

    std::array<domain::ChoiceItem, 4> choices{};
    uint8_t choice_count = 0;
    while (sqlite3_step(cstmt) == SQLITE_ROW && choice_count < 4) {
        const char* disp = reinterpret_cast<const char*>(sqlite3_column_text(cstmt, 0));
        int correct_flag = sqlite3_column_int(cstmt, 1);
        choices[choice_count] = domain::ChoiceItem(disp ? disp : "", correct_flag == 1);
        choice_count++;
    }
    sqlite3_finalize(cstmt);

    domain::Tone tone = static_cast<domain::Tone>(tone_val);
    domain::PhonicsSyllable syllable(onset, rhyme, tone, full_word);
    domain::MissingPart missing = (miss_part == "RHYME") ? domain::MissingPart::RHYME : domain::MissingPart::ONSET;

    return domain::StoryBeat(
        page_number,
        m_total_pages,
        std::move(lead_text),
        std::move(init_img),
        std::move(trans_img),
        syllable,
        missing,
        choices,
        choice_count
    );
}

} // namespace gach::adapters::db