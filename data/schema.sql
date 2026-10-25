-- GACH Story Phonics Engine - 3NF Database Schema
-- Optimized for Kindle FAT32 storage & 500KB cache cap

PRAGMA foreign_keys = ON;

-- 1. Bảng Truyện (Stories)
CREATE TABLE IF NOT EXISTS stories (
    id TEXT PRIMARY KEY,
    title TEXT NOT NULL,
    total_pages INTEGER NOT NULL CHECK(total_pages > 0),
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

-- 2. Bảng Trang Truyện (Pages)
CREATE TABLE IF NOT EXISTS pages (
    story_id TEXT NOT NULL,
    page_number INTEGER NOT NULL CHECK(page_number > 0),
    lead_text TEXT NOT NULL,
    initial_image_path TEXT NOT NULL,
    transform_image_path TEXT NOT NULL,
    PRIMARY KEY (story_id, page_number),
    FOREIGN KEY (story_id) REFERENCES stories(id) ON DELETE CASCADE
);

-- 3. Bảng Mục Tiêu Âm Vị (Phonics Targets)
CREATE TABLE IF NOT EXISTS phonics_targets (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    story_id TEXT NOT NULL,
    page_number INTEGER NOT NULL,
    full_word TEXT NOT NULL,
    onset TEXT NOT NULL,
    rhyme TEXT NOT NULL,
    tone INTEGER NOT NULL CHECK(tone BETWEEN 0 AND 5),
    missing_part TEXT NOT NULL CHECK(missing_part IN ('ONSET', 'RHYME')),
    target_text TEXT NOT NULL,
    FOREIGN KEY (story_id, page_number) REFERENCES pages(story_id, page_number) ON DELETE CASCADE
);

-- 4. Bảng Lựa Chọn Mảnh Ghép (Choices)
CREATE TABLE IF NOT EXISTS choices (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    target_id INTEGER NOT NULL,
    display_text TEXT NOT NULL,
    is_correct INTEGER NOT NULL CHECK(is_correct IN (0, 1)),
    FOREIGN KEY (target_id) REFERENCES phonics_targets(id) ON DELETE CASCADE
);

-- Chỉ mục tối ưu truy vấn nạp trang theo thứ tự nhịp truyện
CREATE INDEX IF NOT EXISTS idx_pages_order ON pages(story_id, page_number);
CREATE INDEX IF NOT EXISTS idx_phonics_lookup ON phonics_targets(story_id, page_number);
CREATE INDEX IF NOT EXISTS idx_choices_target ON choices(target_id);
