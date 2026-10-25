-- Kịch bản trọn vẹn 16 trang tập truyện bạn Mít: "Một Ngày Tử Tế Của Bé Mít"
-- Triết lý: Gốc (Nhân lành) -> Âm (Ái ngữ) -> Cảnh (Duyên tốt) -> Hạnh (Quả ngọt)

DELETE FROM choices WHERE target_id IN (SELECT id FROM phonics_targets WHERE story_id = 'mit_ngay_tu_te');
DELETE FROM phonics_targets WHERE story_id = 'mit_ngay_tu_te';
DELETE FROM pages WHERE story_id = 'mit_ngay_tu_te';
DELETE FROM stories WHERE id = 'mit_ngay_tu_te';

INSERT OR REPLACE INTO stories (id, title, total_pages) VALUES 
('mit_ngay_tu_te', 'Một ngày tử tế của bé Mít', 16);

-- TRANG 1: Chào ông bà (Âm đầu ghép: ch - chào)
INSERT OR REPLACE INTO pages (story_id, page_number, lead_text, initial_image_path, transform_image_path) VALUES
('mit_ngay_tu_te', 1, 
 'Sáng sớm mai, ánh nắng ban mai chiếu qua khung cửa sổ. Bạn Mít thức dậy liền khoanh tay lễ phép chào ông bà...',
 'assets/images/page1_dry.png', 'assets/images/page1_bloom.png');
INSERT OR REPLACE INTO phonics_targets (id, story_id, page_number, full_word, onset, rhyme, tone, missing_part, target_text) VALUES
(1, 'mit_ngay_tu_te', 1, 'chào', 'ch', 'ào', 1, 'ONSET', 'ch');
INSERT OR REPLACE INTO choices (target_id, display_text, is_correct) VALUES
(1, 'ch', 1), (1, 'nh', 0), (1, 'tr', 0);

-- TRANG 2: Nhặt rau giúp mẹ (Vần đơn/ghép: au - rau)
INSERT OR REPLACE INTO pages (story_id, page_number, lead_text, initial_image_path, transform_image_path) VALUES
('mit_ngay_tu_te', 2, 
 'Mẹ đang chuẩn bị bữa cơm trưa trong bếp ấm. Bạn Mít ngồi bên chiếc rổ nhỏ, chăm chỉ giúp mẹ nhặt từng cọng rau tươi ngon...',
 'assets/images/page2_dry.png', 'assets/images/page2_bloom.png');
INSERT OR REPLACE INTO phonics_targets (id, story_id, page_number, full_word, onset, rhyme, tone, missing_part, target_text) VALUES
(2, 'mit_ngay_tu_te', 2, 'rau', 'r', 'au', 0, 'RHYME', 'au');
INSERT OR REPLACE INTO choices (target_id, display_text, is_correct) VALUES
(2, 'au', 1), (2, 'an', 0), (2, 'am', 0);

-- TRANG 3: Cho mèo ăn cơm (Vần ghép: ơm - cơm)
INSERT OR REPLACE INTO pages (story_id, page_number, lead_text, initial_image_path, transform_image_path) VALUES
('mit_ngay_tu_te', 3, 
 'Chú mèo mướp kêu meo meo ngoan ngoãn dưới chân bàn. Bạn Mít nhẹ nhàng chia một phần cơm ấm thơm phức cho mèo con...',
 'assets/images/page3_dry.png', 'assets/images/page3_bloom.png');
INSERT OR REPLACE INTO phonics_targets (id, story_id, page_number, full_word, onset, rhyme, tone, missing_part, target_text) VALUES
(3, 'mit_ngay_tu_te', 3, 'cơm', 'c', 'ơm', 0, 'RHYME', 'ơm');
INSERT OR REPLACE INTO choices (target_id, display_text, is_correct) VALUES
(3, 'ơm', 1), (3, 'ơn', 0), (3, 'om', 0);

-- TRANG 4: Nhặt rác bỏ thùng (Âm đầu: r - rác)
INSERT OR REPLACE INTO pages (story_id, page_number, lead_text, initial_image_path, transform_image_path) VALUES
('mit_ngay_tu_te', 4, 
 'Thấy một mẩu giấy bánh ai đó đánh rơi trên sân gạch, Mít cúi xuống nhặt mẩu rác bỏ gọn gàng vào thùng rác công cộng...',
 'assets/images/page4_dry.png', 'assets/images/page4_bloom.png');
INSERT OR REPLACE INTO phonics_targets (id, story_id, page_number, full_word, onset, rhyme, tone, missing_part, target_text) VALUES
(4, 'mit_ngay_tu_te', 4, 'rác', 'r', 'ác', 2, 'ONSET', 'r');
INSERT OR REPLACE INTO choices (target_id, display_text, is_correct) VALUES
(4, 'r', 1), (4, 'd', 0), (4, 'g', 0);

-- TRANG 5: Tưới cây cho bà (Âm đầu: t - tưới)
INSERT OR REPLACE INTO pages (story_id, page_number, lead_text, initial_image_path, transform_image_path) VALUES
('mit_ngay_tu_te', 5, 
 'Trưa hè nắng gắt, khóm hoa của bà đang khô héo vì khát nước. Bạn Mít cầm bình nhỏ tươi cười ra tưới mát cho cây...',
 'assets/images/page5_dry.png', 'assets/images/page5_bloom.png');
INSERT OR REPLACE INTO phonics_targets (id, story_id, page_number, full_word, onset, rhyme, tone, missing_part, target_text) VALUES
(5, 'mit_ngay_tu_te', 5, 'tưới', 't', 'ươi', 2, 'ONSET', 't');
INSERT OR REPLACE INTO choices (target_id, display_text, is_correct) VALUES
(5, 't', 1), (5, 'c', 0), (5, 'b', 0);

-- TRANG 6: Xếp dép ngay ngắn (Âm đầu: d - dép)
INSERT OR REPLACE INTO pages (story_id, page_number, lead_text, initial_image_path, transform_image_path) VALUES
('mit_ngay_tu_te', 6, 
 'Bố đi làm về mệt mỏi, đôi dép để nghiêng trên bậc thềm. Mít chạy lại cúi mình xếp ngay ngắn đôi dép của bố...',
 'assets/images/page6_dry.png', 'assets/images/page6_bloom.png');
INSERT OR REPLACE INTO phonics_targets (id, story_id, page_number, full_word, onset, rhyme, tone, missing_part, target_text) VALUES
(6, 'mit_ngay_tu_te', 6, 'dép', 'd', 'ép', 2, 'ONSET', 'd');
INSERT OR REPLACE INTO choices (target_id, display_text, is_correct) VALUES
(6, 'd', 1), (6, 'đ', 0), (6, 'v', 0);

-- TRANG 7: Chia quả táo (Vần ghép: áo - táo)
INSERT OR REPLACE INTO pages (story_id, page_number, lead_text, initial_image_path, transform_image_path) VALUES
('mit_ngay_tu_te', 7, 
 'Bà cho Mít một quả táo đỏ thơm ngọt ngào. Thấy bạn nhỏ bên nhà chưa có quà, Mít liền vui vẻ chia đôi quả táo cho bạn...',
 'assets/images/page7_dry.png', 'assets/images/page7_bloom.png');
INSERT OR REPLACE INTO phonics_targets (id, story_id, page_number, full_word, onset, rhyme, tone, missing_part, target_text) VALUES
(7, 'mit_ngay_tu_te', 7, 'táo', 't', 'áo', 2, 'RHYME', 'áo');
INSERT OR REPLACE INTO choices (target_id, display_text, is_correct) VALUES
(7, 'áo', 1), (7, 'éo', 0), (7, 'ao', 0);

-- TRANG 8: Dắt tay em nhỏ (Vần: em - em)
INSERT OR REPLACE INTO pages (story_id, page_number, lead_text, initial_image_path, transform_image_path) VALUES
('mit_ngay_tu_te', 8, 
 'Em bé tập đi chập chững suýt ngã ở bậc cửa. Mít vội chìa bàn tay ấm áp ra dắt tay em từng bước an toàn...',
 'assets/images/page8_dry.png', 'assets/images/page8_bloom.png');
INSERT OR REPLACE INTO phonics_targets (id, story_id, page_number, full_word, onset, rhyme, tone, missing_part, target_text) VALUES
(8, 'mit_ngay_tu_te', 8, 'em', '', 'em', 0, 'RHYME', 'em');
INSERT OR REPLACE INTO choices (target_id, display_text, is_correct) VALUES
(8, 'em', 1), (8, 'en', 0), (8, 'ep', 0);

-- TRANG 9: Lau sạch chiếc bàn (Âm đầu: b - bàn)
INSERT OR REPLACE INTO pages (story_id, page_number, lead_text, initial_image_path, transform_image_path) VALUES
('mit_ngay_tu_te', 9, 
 'Sau giờ tô màu, mặt bàn dính vài vệt chì màu. Mít tự giác nhúng khăn lau sạch chiếc bàn học của mình thật tinh tươm...',
 'assets/images/page9_dry.png', 'assets/images/page9_bloom.png');
INSERT OR REPLACE INTO phonics_targets (id, story_id, page_number, full_word, onset, rhyme, tone, missing_part, target_text) VALUES
(9, 'mit_ngay_tu_te', 9, 'bàn', 'b', 'àn', 1, 'ONSET', 'b');
INSERT OR REPLACE INTO choices (target_id, display_text, is_correct) VALUES
(9, 'b', 1), (9, 'p', 0), (9, 'm', 0);

-- TRANG 10: Hái lá thơm cho bà (Âm đầu: l - lá)
INSERT OR REPLACE INTO pages (story_id, page_number, lead_text, initial_image_path, transform_image_path) VALUES
('mit_ngay_tu_te', 10, 
 'Bà bị cảm ho, cần nồi nước lá thơm ấm áp. Mít ra sau vườn cẩn thận ngắt từng ngọn lá tía tô thơm ngát đem vào biếu bà...',
 'assets/images/page10_dry.png', 'assets/images/page10_bloom.png');
INSERT OR REPLACE INTO phonics_targets (id, story_id, page_number, full_word, onset, rhyme, tone, missing_part, target_text) VALUES
(10, 'mit_ngay_tu_te', 10, 'lá', 'l', 'á', 2, 'ONSET', 'l');
INSERT OR REPLACE INTO choices (target_id, display_text, is_correct) VALUES
(10, 'l', 1), (10, 'n', 0), (10, 'h', 0);

-- TRANG 11: Che ô khi trời mưa (Vần đôi: ưa - mưa)
INSERT OR REPLACE INTO pages (story_id, page_number, lead_text, initial_image_path, transform_image_path) VALUES
('mit_ngay_tu_te', 11, 
 'Trời đổ cơn mưa rào rải đều trên mái ngói. Mít cầm chiếc ô nhỏ chạy ra che mưa đón bạn cùng bước vào hiên nhà khô ráo...',
 'assets/images/page11_dry.png', 'assets/images/page11_bloom.png');
INSERT OR REPLACE INTO phonics_targets (id, story_id, page_number, full_word, onset, rhyme, tone, missing_part, target_text) VALUES
(11, 'mit_ngay_tu_te', 11, 'mưa', 'm', 'ưa', 0, 'RHYME', 'ưa');
INSERT OR REPLACE INTO choices (target_id, display_text, is_correct) VALUES
(11, 'ưa', 1), (11, 'ua', 0), (11, 'ơ', 0);

-- TRANG 12: Giữ yên lặng cho em ngủ (Âm đầu ghép: ng - ngủ)
INSERT OR REPLACE INTO pages (story_id, page_number, lead_text, initial_image_path, transform_image_path) VALUES
('mit_ngay_tu_te', 12, 
 'Bé em đang say giấc nồng trong chiếc nôi êm. Bạn Mít bước đi khẽ khàng rón rén, giữ cho em giấc ngủ thật yên bình...',
 'assets/images/page12_dry.png', 'assets/images/page12_bloom.png');
INSERT OR REPLACE INTO phonics_targets (id, story_id, page_number, full_word, onset, rhyme, tone, missing_part, target_text) VALUES
(12, 'mit_ngay_tu_te', 12, 'ngủ', 'ng', 'ủ', 3, 'ONSET', 'ng');
INSERT OR REPLACE INTO choices (target_id, display_text, is_correct) VALUES
(12, 'ng', 1), (12, 'gh', 0), (12, 'nh', 0);

-- TRANG 13: Bắt sâu cho hoa (Vần ghép: âu - sâu)
INSERT OR REPLACE INTO pages (story_id, page_number, lead_text, initial_image_path, transform_image_path) VALUES
('mit_ngay_tu_te', 13, 
 'Trên cành búp non có một chú sâu đang gặm lá. Mít dùng chiếc que nhỏ gạt nhẹ chú sâu ra gốc cây an toàn để bảo vệ chồi non...',
 'assets/images/page13_dry.png', 'assets/images/page13_bloom.png');
INSERT OR REPLACE INTO phonics_targets (id, story_id, page_number, full_word, onset, rhyme, tone, missing_part, target_text) VALUES
(13, 'mit_ngay_tu_te', 13, 'sâu', 's', 'âu', 0, 'RHYME', 'âu');
INSERT OR REPLACE INTO choices (target_id, display_text, is_correct) VALUES
(13, 'âu', 1), (13, 'ây', 0), (13, 'au', 0);

-- TRANG 14: Rót nước mời ông (Vần ghép 3 ký tự: ước - nước)
INSERT OR REPLACE INTO pages (story_id, page_number, lead_text, initial_image_path, transform_image_path) VALUES
('mit_ngay_tu_te', 14, 
 'Ông ngồi nghỉ ngơi bên chiếc bàn trà. Mít rót một cốc nước ấm, dùng hai bàn tay nhỏ cung kính dâng mời ông uống...',
 'assets/images/page14_dry.png', 'assets/images/page14_bloom.png');
INSERT OR REPLACE INTO phonics_targets (id, story_id, page_number, full_word, onset, rhyme, tone, missing_part, target_text) VALUES
(14, 'mit_ngay_tu_te', 14, 'nước', 'n', 'ước', 2, 'RHYME', 'ước');
INSERT OR REPLACE INTO choices (target_id, display_text, is_correct) VALUES
(14, 'ước', 1), (14, 'ươc', 0), (14, 'uốc', 0);

-- TRANG 15: Cất đồ chơi gọn gàng (Âm đầu: g - gọn)
INSERT OR REPLACE INTO pages (story_id, page_number, lead_text, initial_image_path, transform_image_path) VALUES
('mit_ngay_tu_te', 15, 
 'Chơi xong các khối gỗ màu sắc, Mít cẩn thận xếp từng món đồ chơi vào giỏ để căn phòng luôn ngăn nắp và gọn gàng...',
 'assets/images/page15_dry.png', 'assets/images/page15_bloom.png');
INSERT OR REPLACE INTO phonics_targets (id, story_id, page_number, full_word, onset, rhyme, tone, missing_part, target_text) VALUES
(15, 'mit_ngay_tu_te', 15, 'gọn', 'g', 'ọn', 5, 'ONSET', 'g');
INSERT OR REPLACE INTO choices (target_id, display_text, is_correct) VALUES
(15, 'g', 1), (15, 'c', 0), (15, 'k', 0);

-- TRANG 16: Ngôi nhà ấm cúng (Âm đầu ghép: nh - nhà)
INSERT OR REPLACE INTO pages (story_id, page_number, lead_text, initial_image_path, transform_image_path) VALUES
('mit_ngay_tu_te', 16, 
 'Trời về chiều tối, cả gia đình quây quần sum họp ấm cúng bên nhau. Nụ cười rạng ngời hạnh phúc khắp ngôi nhà nhỏ thân yêu...',
 'assets/images/page16_dry.png', 'assets/images/page16_bloom.png');
INSERT OR REPLACE INTO phonics_targets (id, story_id, page_number, full_word, onset, rhyme, tone, missing_part, target_text) VALUES
(16, 'mit_ngay_tu_te', 16, 'nhà', 'nh', 'à', 1, 'ONSET', 'nh');
INSERT OR REPLACE INTO choices (target_id, display_text, is_correct) VALUES
(16, 'nh', 1), (16, 'ch', 0), (16, 'th', 0);
