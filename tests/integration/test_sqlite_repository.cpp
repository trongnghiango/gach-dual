#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include "adapters/db/sqlite_story_repository.hpp"

using namespace gach::adapters::db;
using namespace gach::domain;

std::string read_file(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + path);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void test_sqlite_repository_flow() {
    SqliteStoryRepository repo(":memory:");
    assert(repo.initialize() == true);

    // Nạp DDL Schema và Seed Data
    std::string schema = read_file("data/schema.sql");
    std::string seed = read_file("data/seed_story_mit.sql");
    assert(repo.execute_sql(schema) == true);
    assert(repo.execute_sql(seed) == true);

    // 1. Kiểm tra load story
    assert(repo.load_story("mit_tuoi_cay") == true);
    assert(repo.total_pages() == 3);
    assert(repo.story_title() == "Mít tưới cây cho bà");
    assert(repo.current_story_id() == "mit_tuoi_cay");

    // 2. Kiểm tra Page 1
    auto beat1 = repo.get_beat(1);
    assert(beat1.has_value());
    assert(beat1->page_number() == 1);
    assert(beat1->lead_text().find("Trưa hè nắng gắt") != std::string::npos);
    assert(beat1->syllable().full_word() == "tưới");
    assert(beat1->syllable().onset() == "t");
    assert(beat1->syllable().rhyme() == "ươi");
    assert(beat1->syllable().tone() == Tone::SAC);
    assert(beat1->missing_part() == MissingPart::ONSET);
    assert(beat1->choice_count() == 3);
    assert(beat1->choices()[0].display_text() == "t" && beat1->choices()[0].is_correct == true);
    assert(beat1->choices()[1].display_text() == "c" && beat1->choices()[1].is_correct == false);
    assert(beat1->choices()[2].display_text() == "b" && beat1->choices()[2].is_correct == false);

    // 3. Kiểm tra Page 2
    auto beat2 = repo.get_beat(2);
    assert(beat2.has_value());
    assert(beat2->syllable().full_word() == "cây");
    assert(beat2->syllable().rhyme() == "ây");
    assert(beat2->missing_part() == MissingPart::RHYME);
    assert(beat2->choices()[0].display_text() == "ây" && beat2->choices()[0].is_correct == true);

    // 4. Kiểm tra Page 3
    auto beat3 = repo.get_beat(3);
    assert(beat3.has_value());
    assert(beat3->syllable().full_word() == "bà");
    assert(beat3->syllable().onset() == "b");
    assert(beat3->missing_part() == MissingPart::ONSET);
    assert(beat3->choices()[0].display_text() == "b" && beat3->choices()[0].is_correct == true);

    // 5. Kiểm tra biên ngoại lệ
    assert(!repo.get_beat(0).has_value());
    assert(!repo.get_beat(4).has_value());
    assert(!repo.load_story("truyen_khong_ton_tai"));

    // 6. Benchmark tốc độ truy vấn nhịp truyện (< 2ms yêu cầu)
    auto start = std::chrono::high_resolution_clock::now();
    const int iterations = 1000;
    for (int i = 0; i < iterations; ++i) {
        uint32_t p = (i % 3) + 1;
        auto b = repo.get_beat(p);
        (void)b;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    double avg_ms = elapsed.count() / iterations;
    std::cout << "  ✓ 1000 queries completed in " << elapsed.count() << " ms (Avg: " << avg_ms << " ms/query)" << std::endl;
    assert(avg_ms < 0.2 && "Query must be under 0.2ms in-memory");

    std::cout << "  ✓ test_sqlite_repository_flow passed" << std::endl;
}

int main() {
    std::cout << "--- Running SQLite Repository Integration Tests ---" << std::endl;
    test_sqlite_repository_flow();
    std::cout << "All SQLite Repository Integration Tests passed!" << std::endl;
    return 0;
}