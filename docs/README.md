# TÀI LIỆU KỸ THUẬT DỰ ÁN GACH (KINDLE-MÍT STORY PHONICS ENGINE)

Chào mừng bạn đến với kho tài liệu kỹ thuật của dự án **GACH** (Kindle-Mít) dành cho máy đọc sách Kindle Paperwhite 2/3 (PW2/PW3).

---

## 1. MỤC LỤC TÀI LIỆU

### 1.1. Bộ Tiêu Chuẩn Kỹ Nghệ AI & Thiết Bị Nhúng (`docs/standards/`)
Bộ tiêu chuẩn kỹ thuật cốt lõi được điều chỉnh tối ưu cho dự án native C++17 và phần cứng E-Ink Kindle:
1. [`EMBEDDED-KINDLE-STANDARD.md`](standards/EMBEDDED-KINDLE-STANDARD.md): Ràng buộc phần cứng, tối ưu tài nguyên (RAM < 12MB, Zero-Heap trong Hot Path, E-Ink refresh, FAT32 `access(F_OK)`).
2. [`AI-ENGINEERING-CONSTITUTION.md`](standards/AI-ENGINEERING-CONSTITUTION.md): Kỷ luật tối cao Zero-Hack, Root-cause over symptom, trách nhiệm kỹ thuật toàn diện.
3. [`AUTONOMOUS-ENGINEERING-LOOP.md`](standards/AUTONOMOUS-ENGINEERING-LOOP.md): Chu trình 7 pha tự chủ từ Intake đến DoD.
4. [`ARCHITECTURE.md`](standards/ARCHITECTURE.md): Clean Architecture 4 tầng, The Dependency Rule, phân lập hoàn toàn Domain khỏi IO/DB/UI.
5. [`BUSINESS-RULES.md`](standards/BUSINESS-RULES.md): Rich Domain Model, Value Objects âm vị tiếng Việt bất biến trên Stack, Unified 4-Beat Flow FSM, triết lý Anti-Dopamine.
6. [`TESTING-STANDARD.md`](standards/TESTING-STANDARD.md): Kim tự tháp kiểm thử 70/20/10, TDD Red-Green-Refactor, Quality Gates không rò rỉ bộ nhớ.
7. [`UI-UX-STANDARD.md`](standards/UI-UX-STANDARD.md): 4 trạng thái giao diện bắt buộc, quy chuẩn làm mới màn hình E-Ink, công thức hitbox cho ngón tay trẻ 3–6 tuổi.

### 1.2. Kiến Trúc & Thiết Kế Chi Tiết (`docs/architecture/`)
- [`story-flow-fsm.md`](architecture/story-flow-fsm.md): Đặc tả máy trạng thái dòng chảy 4 nhịp thống nhất.
- [`sqlite-3nf-schema.md`](architecture/sqlite-3nf-schema.md): Lược đồ cơ sở dữ liệu SQLite3 3NF cho kịch bản truyện và âm vị.

### 1.3. Lịch Sử Bàn Giao (`docs/handoff/`)
- [`latest.md`](handoff/latest.md): Tài liệu bàn giao đầy đủ từ giai đoạn nghiên cứu và thiết kế ban đầu.

---

## 2. QUY CHUẨN CẬP NHẬT TÀI LIỆU
- **Tài liệu phải tiến hóa cùng mã nguồn**: Bất kỳ sự thay đổi nào về lược đồ database, state machine, hay giao diện ports đều phải được cập nhật đồng bộ vào thư mục `docs/`.
- **Cơ chế cập nhật thường xuyên**: Mỗi chu trình phát triển (Feature/Refactor) đều kết thúc bằng việc đối chiếu và đồng bộ tài liệu theo đúng Tiêu chuẩn Kỹ nghệ Phần mềm.
