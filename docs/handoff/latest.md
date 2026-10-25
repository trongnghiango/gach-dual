# Handoff: Dự án GACH (Kindle-Mít) — Story Phonics & Moral Agency Engine Cho Kindle PW2/PW3

## Focus
Toàn bộ quá trình nghiên cứu, thảo luận triết lý giáo dục (Nhân - Duyên - Quả, Ngũ Uẩn, Anti-Dopamine), thiết kế kiến trúc phần mềm C++17 Clean Architecture 4 tầng, tối ưu hóa phần cứng E-Ink cho máy Kindle Paperwhite 2/3 (PW2/PW3), thiết kế cơ sở dữ liệu quan hệ SQLite3 (`story_engine.db`), quy chuẩn tài nguyên hình ảnh Grayscale PNG 1-channel, đối chiếu các bộ tiêu chuẩn `/home/ka/Documents/AI_docs/standards/`, và định danh dự án **GACH**.

## Handoff reason
user-initiated

## Repo state
- **Branch**: `main`
- **HEAD commit**: `1d8e00e8770e5dbe9cd924fa0efd248a7caa1329`
- **Git status short**:
```
?? docs/
```

## State
Toàn bộ giai đoạn nghiên cứu, thiết kế kiến trúc và đặc tả kỹ thuật cho dự án mới **GACH** đã hoàn tất xuất sắc và được sự đồng thuận 100% của người dùng. Thư mục dự án mới đã được khởi tạo sẵn sàng tại `/home/ka/Repos/github.com/trongnghiango/gach-kindle` để chuyển giao sang session mới bắt tay vào hiện thực hóa mã nguồn từ đầu.

## Verification
- **Chess Engine Benchmark (KindleChess Host Baseline)**: PASS (6/6 tests passing, 10.000 lần sinh nước đi hoàn thành trong ~15.5 ms).
  - Lệnh kiểm tra: `g++ -std=c++17 -O2 -I src src/chess.cpp tests/test_chess.cpp -o /tmp/test_chess_bin && /tmp/test_chess_bin && rm -f /tmp/test_chess_bin`
- **Module Linking Host Build**: PASS (biên dịch toàn bộ 6 module C++ thành công).
  - Lệnh kiểm tra: `g++ -std=c++17 -O2 -I src src/main.cpp src/app_controller.cpp src/ui_render.cpp src/ui_layout.cpp src/chess.cpp src/config.cpp src/engine.cpp $(pkg-config --cflags --libs gtk+-3.0) -o /tmp/test_bin && rm -f /tmp/test_bin`

## Constraints
- **Phần cứng Kindle Paperwhite 2/3 (PW2/PW3)**:
  - Chip Freescale i.MX6 SoloLite (Cortex-A9, 1 nhân 1.0 GHz, 32-bit ARMv7).
  - RAM tổng 512MB, nhưng hệ thống Amazon chiếm phần lớn, **chỉ còn ~35-45MB RAM khả dụng**. Toàn bộ app GACH phải khống chế trần RAM $< 12\text{ MB}$.
  - Hoàn toàn **KHÔNG có loa ngoài và jack tai nghe 3.5mm**. Tranh ảnh và ngữ cảnh đóng vai trò trực quan hóa âm vị (Visual Phonics), cha mẹ là người đồng hành phát âm tự nhiên.
  - Phân vùng `/mnt/us` mount dạng **FAT32**. Mọi kiểm tra file thực thi phải dùng `access(F_OK)`, cấm dùng `access(X_OK)`.
- **Triết lý Sư phạm & Tâm lý Trẻ 3–6 tuổi**:
  - Bé đã thuộc bảng chữ cái $\rightarrow$ Trọng tâm là ghép vần (Blending) và đọc hiểu câu chuyện.
  - Không điểm số, không sao vàng, không phạt, không âm thanh kích động $\rightarrow$ Triệt tiêu cơ chế gây nghiện Dopamine ngắn hạn (Anti-Dopamine).
  - Ngôn từ 100% trong trẻo, gần gũi, đời thường $\rightarrow$ Tuyệt đối không dùng thuật ngữ Phật học/triết học hàn lâm trong app.
- **Kỷ luật Kỹ thuật (Tuân thủ `/home/ka/Documents/AI_docs/standards/`)**:
  - Tuân thủ Clean Architecture 4 tầng, Rich Domain Model, Value Objects bất biến trên Stack.
  - Kim tự tháp kiểm thử 70/20/10 với nguyên tắc Zero-Hack (TDD viết test trước, test fail trước khi viết code).
  - Zero-Dynamic-Allocation trong hot path (không dùng `new`/`malloc` trong luồng vẽ Cairo và hitbox cảm ứng).

## Risk context
Tier 1 (Thấp) - Chuyển giao kiến trúc và chuẩn bị khởi tạo repository mới, không ảnh hưởng đến bất kỳ hệ thống production nào.

## Artifacts
1. **Dự án mới (Đã tạo thư mục)**:
   - `/home/ka/Repos/github.com/trongnghiango/gach-kindle`
2. **Bộ tiêu chuẩn kỹ thuật bắt buộc**:
   - `/home/ka/Documents/AI_docs/standards/AI-ENGINEERING-CONSTITUTION.md`
   - `/home/ka/Documents/AI_docs/standards/AUTONOMOUS-ENGINEERING-LOOP.md`
   - `/home/ka/Documents/AI_docs/standards/ARCHITECTURE.md`
   - `/home/ka/Documents/AI_docs/standards/BUSINESS-RULES.md`
   - `/home/ka/Documents/AI_docs/standards/TESTING-STANDARD.md`
   - `/home/ka/Documents/AI_docs/standards/UI-UX-STANDARD.md`
3. **Mã nguồn tham khảo kiến trúc nhúng**:
   - Repository hiện tại `/home/ka/Repos/github.com/trongnghiango/Kindle-Chess` (6 module Clean Architecture C++17, script đóng gói KUAL, pipeline GitHub Actions).

## Decisions
1. **Tên Dự Án: GACH** (Thương hiệu & Triết lý Backronym song hành):
   - **G**iác quan (Mắt thấy nét mực, tay chạm màn hình - Sắc/Thọ).
   - **Â**m vị (Nhận diện thanh âm, vần điệu - Tưởng).
   - **C**ảm nhận (Thấu cảm hoàn cảnh của bạn Mít - Thức).
   - **H**ành động (Chạm mở khóa điều tử tế - Hành).
   - *Song hành cùng trục Nhân - Duyên*: **G**ốc (Nhân) - **Â**m (Lời nói) - **C**ảnh (Duyên) - **H**ạnh (Quả).
2. **Cơ sở dữ liệu SQLite3 (`story_engine.db`) thay cho JSON**:
   - Sử dụng quan hệ bảng 3NF với khóa ngoại cứng `PRAGMA foreign_keys = ON;`.
   - SQLite cache khóa cứng ở mức `-500` (500KB RAM), dùng `journal_mode = DELETE` chống phân mảnh flash FAT32.
   - Nội dung tách biệt hoàn toàn khỏi Engine C++ (Content Decoupling).
3. **Máy Trạng Thái Dòng Chảy Chung Duy Nhất (Unified 4-Beat Flow FSM)**:
   - Thay vì viết FSM riêng cho từng truyện, dùng 1 FSM 4 nhịp duy nhất trong `app_controller.cpp`:
     * Nhịp 1 (`OBSERVE`): Đọc lời dẫn & ngắm tranh 2.5s (khóa cảm ứng chống bấm loạn).
     * Nhịp 2 (`AWAIT_INPUT`): Hiện mảnh ghép vần khuyết, bé chạm tương tác (hỗ trợ tự dẫn lối sau 6s).
     * Nhịp 3 (`TRANSFORM`): Từ ghép hoàn chỉnh, tranh đổi sang cảnh quả ngọt tử tế, khoảng lặng thì thầm.
     * Nhịp 4 (`COMPLETED`): Nút mũi tên xuất hiện để bé chủ động sang trang tiếp.
4. **Quy chuẩn Đồ họa Tài nguyên**:
   - Lưu trữ gốc trên máy tính: SVG/Vector.
   - Đóng gói vào Kindle: **PNG Grayscale 1-channel (8-bit hoặc 4-bit 16 mức xám)** đúng kích thước vật lý khung tranh ($1072 \times 1014\text{ px}$ trên PW3, $758 \times 716\text{ px}$ trên PW2). RAM đệm ảnh chỉ tốn $1.1\text{ MB}$, tốc độ nạp Cairo $< 15\text{ ms}$.

## Next steps (Dành cho Session Mới)
1. **Thiết lập Workspace Mới**:
   - Mở workspace tại `/home/ka/Repos/github.com/trongnghiango/gach-kindle`.
   - Khởi tạo Git repo (`git init`), cấu hình `.gitignore` chuẩn C++/Kindle.
2. **Khởi tạo Bộ Khung Dự Án (Scaffolding)**:
   - Thiết lập cấu trúc thư mục Clean Architecture:
     * `src/domain/` (Pure logic âm tiết, FSM dòng chảy)
     * `src/application/` (Use cases, Ports interfaces)
     * `src/adapters/` (SQLite repo, Cairo renderer, GTK input)
     * `src/infrastructure/` (File loader, OS hooks, KUAL launcher)
     * `tests/` (Bộ unit test C++ theo chuẩn TDD)
     * `scripts/` (Build script ARMv7 cross-compile, package kual zip)
     * `data/` (Schema SQLite `schema.sql`, file mẫu `story_engine.db`)
3. **Hiện thực hóa TDD cho Domain & Schema**:
   - Viết test suite cho `PhonicsSyllable` và `FlowFSM` trước khi code.
   - Khởi tạo script Python hoặc SQLite CLI tạo database mẫu cho tập truyện Mít đầu tiên (*"Mít tưới cây cho bà"*).
4. **Dựng Engine Cairo & GTK**:
   - Tái sử dụng các kinh nghiệm render E-Ink không nhấp nháy (Dirty rect refresh) từ `KindleChess`.
5. **Đóng gói KUAL & Kiểm thử trên Kindle PW3**.

## Blockers
N/A

## Prior handoff
`docs/handoff/2026-10-25-105120_handoff-kindle-chess-audit-refactor-pw3-optimizations.md`

## Suggested skills
- `ka-plan`: Tiếp tục phân tích chi tiết nếu phát sinh rẽ nhánh nghiệp vụ.
- `ka-be`: Chuẩn hóa Clean Architecture C++ và triển khai mã nguồn.
- `ka-test`: Viết unit test và integration test theo kim tự tháp 70/20/10.
- `ka-ops`: Thiết lập quy trình build chéo (cross-compilation) ARMv7 cho Kindle PW2/PW3.
- `git-guardian`: Giám sát an toàn tuyệt đối trước mọi thao tác Git trong repo mới.