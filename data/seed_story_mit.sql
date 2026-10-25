-- Kịch bản mẫu tập truyện Mít đầu tiên: "Mít tưới cây cho bà"
-- Triết lý: Nhân (Gốc) -> Âm (Lời nói) -> Cảnh (Duyên) -> Hạnh (Quả)

INSERT INTO stories (id, title, total_pages) VALUES 
('mit_tuoi_cay', 'Mít tưới cây cho bà', 3);

-- Trang 1: Khóm hoa khát nước, Mít tưới cây
INSERT INTO pages (story_id, page_number, lead_text, initial_image_path, transform_image_path) VALUES
('mit_tuoi_cay', 1, 
 'Trưa hè nắng gắt, khóm hoa của bà đang khô héo vì khát nước. Bạn Mít cầm bình nhỏ ra giúp bà...',
 'assets/images/page1_dry.png', 
 'assets/images/page1_bloom.png');

INSERT INTO phonics_targets (id, story_id, page_number, full_word, onset, rhyme, tone, missing_part, target_text) VALUES
(1, 'mit_tuoi_cay', 1, 'tưới', 't', 'ươi', 2, 'ONSET', 't');

INSERT INTO choices (target_id, display_text, is_correct) VALUES
(1, 't', 1),
(1, 'c', 0),
(1, 'b', 0);

-- Trang 2: Nước mát lành thấm đất, cây xanh mướt
INSERT INTO pages (story_id, page_number, lead_text, initial_image_path, transform_image_path) VALUES
('mit_tuoi_cay', 2, 
 'Từng giọt nước mát lành thấm sâu vào lòng đất ấm. Khóm hoa tươi tỉnh, những chiếc lá rung rinh...',
 'assets/images/page2_leaves.png', 
 'assets/images/page2_green.png');

INSERT INTO phonics_targets (id, story_id, page_number, full_word, onset, rhyme, tone, missing_part, target_text) VALUES
(2, 'mit_tuoi_cay', 2, 'cây', 'c', 'ây', 0, 'RHYME', 'ây');

INSERT INTO choices (target_id, display_text, is_correct) VALUES
(2, 'ây', 1),
(2, 'an', 0),
(2, 'ơ', 0);

-- Trang 3: Bà bước ra hiên khen Mít ngoan
INSERT INTO pages (story_id, page_number, lead_text, initial_image_path, transform_image_path) VALUES
('mit_tuoi_cay', 3, 
 'Bà bước ra hiên nhà, nở nụ cười hiền từ xoa đầu bạn Mít. Cả khu vườn rộn rã niềm vui...',
 'assets/images/page3_grandma.png', 
 'assets/images/page3_hug.png');

INSERT INTO phonics_targets (id, story_id, page_number, full_word, onset, rhyme, tone, missing_part, target_text) VALUES
(3, 'mit_tuoi_cay', 3, 'bà', 'b', 'a', 1, 'ONSET', 'b');

INSERT INTO choices (target_id, display_text, is_correct) VALUES
(3, 'b', 1),
(3, 'm', 0),
(3, 'n', 0);
