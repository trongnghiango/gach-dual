# THIẾT KẾ CƠ SỞ DỮ LIỆU SQLITE3 3NF (STORY_ENGINE.DB)

## 1. NGUYÊN TẮC THIẾT KẾ
- Tách biệt 100% nội dung truyện khỏi mã nguồn C++ (Content Decoupling).
- Chuẩn hóa 3NF với ràng buộc khóa ngoại cứng (`PRAGMA foreign_keys = ON;`).
- Tối ưu hóa cho phân vùng FAT32 trên Kindle: `journal_mode = DELETE`, `cache_size = -500` (500KB RAM).

---

## 2. LƯỢC ĐỒ QUAN HỆ BẢNG (3NF SCHEMA)

```sql
-- 1. Bảng Truyện (Stories)
CREATE TABLE IF NOT EXISTS stories (
    id TEXT PRIMARY KEY,               -- e.g. 'mit_tuoi_cay'
    title TEXT NOT NULL,              -- 'Mít tưới cây cho bà'
    total_pages INTEGER NOT NULL,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

-- 2. Bảng Trang Truyện (Pages)
CREATE TABLE IF NOT EXISTS pages (
    story_id TEXT NOT NULL,
    page_number INTEGER NOT NULL,
    lead_text TEXT NOT NULL,          -- Lời dẫn: 'Trời trưa nắng gắt, cây của bà đang héo khô...'
    initial_image_path TEXT NOT NULL, -- Đường dẫn ảnh cảnh nhân/duyên (Grayscale PNG)
    transform_image_path TEXT NOT NULL,-- Đường dẫn ảnh cảnh quả ngọt (Grayscale PNG)
    PRIMARY KEY (story_id, page_number),
    FOREIGN KEY (story_id) REFERENCES stories(id) ON DELETE CASCADE
);

-- 3. Bảng Mục Tiêu Âm Vị (Phonics Targets)
CREATE TABLE IF NOT EXISTS phonics_targets (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    story_id TEXT NOT NULL,
    page_number INTEGER NOT NULL,
    full_word TEXT NOT NULL,          -- 'tưới'
    onset TEXT NOT NULL,              -- 't'
    rhyme TEXT NOT NULL,              -- 'ươi'
    tone TEXT NOT NULL,               -- 'sắc'
    missing_part TEXT NOT NULL CHECK(missing_part IN ('ONSET', 'RHYME')), -- Phần bị khuyết
    target_text TEXT NOT NULL,        -- Giá trị đúng mà bé cần chọn: e.g. 't'
    FOREIGN KEY (story_id, page_number) REFERENCES pages(story_id, page_number) ON DELETE CASCADE
);

-- 4. Bảng Lựa Chọn Mảnh Ghép (Choices)
CREATE TABLE IF NOT EXISTS choices (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    target_id INTEGER NOT NULL,
    display_text TEXT NOT NULL,       -- Ký tự hiển thị trên mảnh ghép: 't', 'c', 'm'
    is_correct INTEGER NOT NULL CHECK(is_correct IN (0, 1)), -- 1 = Đúng, 0 = Lựa chọn phân tán
    FOREIGN KEY (target_id) REFERENCES phonics_targets(id) ON DELETE CASCADE
);
```
