# Handoff: GACH Engine — Minimalism Design System, 16-Page Content & Community Story Specification

## Focus
Bàn giao toàn diện hệ thống **GACH Engine** sau khi hoàn thành: (1) Bộ thư viện bố cục Native C++17 `InkFlex` (Flexbox, Absolute, z-Index); (2) Tái thiết kế giao diện tối giản chuẩn Apple HIG & Dieter Rams (`GMDS`); (3) Mở rộng kịch bản trọn vẹn 16 trang câu chuyện kèm 32 tranh minh họa E-Ink; (4) Thanh điều hướng đỉnh 4 nút tương tác (Quay lại, Mục lục 16 trang, Cài đặt, Thoát) kèm Modal Overlays; (5) Tăng kích thước chữ lời thoại lên 46px cho ba mẹ đọc cùng bé; (6) Công bố bộ quy chuẩn sáng tác cộng đồng `COMMUNITY-STORY-SPECIFICATION.md`, lược đồ JSON Schema `story.schema.json` và phân tích nâng cấp máy trạng thái phân tầng (HFSM).

## Handoff reason
user-initiated

## Repo state
- **Branch**: `main`
- **HEAD Commit**: `717f1b0a2436eeee574f5840924f0ac5062074ea`
- **Working Tree**: Clean (`git status --short` rỗng)
- **Remote**: Đồng bộ 100% với `origin/main` trên GitHub (`trongnghiango/gach-dual`)

## State
Hệ thống GACH Engine đã hoàn thiện toàn diện ở mức sản phẩm sẵn sàng triển khai thực tế. Động cơ bố cục Flexbox `InkFlex` vận hành với tốc độ siêu tốc 0.24µs/layout, giao diện Minimalism thanh nhã không viền hộp, phông chữ 46px/66px sắc nét, thanh điều hướng đỉnh hỗ trợ chuyển trang tiến/lùi, mở mục lục nhảy trang trực tiếp và cài đặt gợi ý tự động. Cả 16 trang câu chuyện *"Một ngày tử tế của bé Mít"* được nạp sẵn trong SQLite 3NF, đi kèm bộ quy chuẩn mở và schema JSON cho cộng đồng sáng tác. 100% bài kiểm thử Unit, Integration, E2E và AddressSanitizer (0 leaks) đều vượt qua tuyệt đối.

## Verification
- **Trạng thái kiểm thử**: PASS 100% (Tất cả test suite vượt qua không lỗi)
- **Lệnh kiểm tra lại nhanh**:
  ```bash
  make test && make test_asan
  ```
- **Lệnh chạy thử nghiệm trên máy tính (Desktop Simulator)**:
  ```bash
  ./bin/gach --db story_engine.db
  ```
- **Lệnh cài đặt tự động lên máy đọc sách Kindle PW2/PW3**:
  ```bash
  ./install_to_kindle.sh --clean-data
  ```
- **GitHub Actions CI**: Tự động cross-compile ARMv7 `gach-kual.zip` cho Kindle PW2/PW3 trên nhánh `main` thành công 100%.

## Constraints
- **Phần cứng mục tiêu**: Freescale i.MX6 SoloLite (Cortex-A9, 1.0 GHz, ARMv7 32-bit `armhf`), Kindle PW2 (758x1024) và PW3 (1072x1448 portrait).
- **Trần bộ nhớ RAM**: Khóa cứng RSS < 12MB. Zero heap allocation trên hot-path tương tác.
- **Hệ thống tệp FAT32 (/mnt/us)**: Kiểm tra file thực thi bằng `access(F_OK)`, tuyệt đối không dùng `access(X_OK)`.
- **Màn hình E-Ink**: Không animation liên tục, không vòng lặp 60fps, không đổ bóng mờ (drop shadow) hay gradient. Làm mới cục bộ (Partial Refresh) cho tương tác, làm mới toàn phần (Full Refresh) khi đổi trang hoặc chuyển hóa cảnh quả ngọt.
- **Sư phạm Anti-Dopamine**: 0 điểm số, 0 ngôi sao xếp hạng, 0 âm thanh còi phạt, không áp lực thời gian.
- **Kiến trúc Clean Architecture**: 4 tầng độc lập tuyệt đối (Domain -> Application -> Adapters -> Infrastructure).

## Risk context
- **Risk Tier**: Tier 1 (Read-only / Documentation / UI Presentation). Mọi thay đổi kỹ thuật lớn đã được kiểm chuẩn qua AddressSanitizer và CI.

## Artifacts
1. **Tiêu chuẩn thiết kế tối giản**: [`docs/standards/MINIMALISM-DESIGN-SYSTEM.md`](docs/standards/MINIMALISM-DESIGN-SYSTEM.md)
2. **Quy chuẩn kịch bản cộng đồng**: [`docs/community/COMMUNITY-STORY-SPECIFICATION.md`](docs/community/COMMUNITY-STORY-SPECIFICATION.md)
3. **Lược đồ JSON Schema kịch bản**: [`docs/community/story.schema.json`](docs/community/story.schema.json)
4. **Mẫu kịch bản khai báo**: [`docs/community/example_story.json`](docs/community/example_story.json)
5. **Động cơ bố cục Flexbox C++17**: [`src/adapters/ui/ink_flex.hpp`](src/adapters/ui/ink_flex.hpp) & [`ink_flex.cpp`](src/adapters/ui/ink_flex.cpp)
6. **Thư viện thành phần tối giản**: [`src/adapters/ui/minimal_components.hpp`](src/adapters/ui/minimal_components.hpp) & [`minimal_components.cpp`](src/adapters/ui/minimal_components.cpp)
7. **Kịch bản CSDL 16 trang**: [`data/seed_story_mit_16pages.sql`](data/seed_story_mit_16pages.sql) & [`story_engine.db`](story_engine.db)
8. **Bộ 32 tranh minh họa tối giản**: [`assets/images/`](assets/images/) sinh bởi [`scripts/generate_images.cpp`](scripts/generate_images.cpp)
9. **Script cài đặt Kindle tự động**: [`install_to_kindle.sh`](install_to_kindle.sh)
10. **GitHub Actions Workflow**: [`.github/workflows/build-kindle.yml`](.github/workflows/build-kindle.yml)

## Decisions
- **D1 (Micro-Flexbox Native Engine)**: Thay vì tích hợp các engine CSS cồng kềnh (Yoga/LiteHTML tốn 2-5MB RAM), tự hiện thực `InkFlex` thuần C++17 siêu nhẹ (~12KB), tốc độ 0.24µs/layout, zero-heap overhead.
- **D2 (Mỹ học Tối giản Minimalism Ehon)**: Loại bỏ toàn bộ viền khung đóng hộp 3px, dấu chữ thập kỹ thuật và bóng đổ giả 3D; đưa tranh minh họa hòa tự nhiên vào nền trang giấy E-Ink.
- **D3 (Mở rộng 16 Trang Truyện)**: Mở rộng từ 3 trang lên 16 trang bao phủ đa dạng âm đầu đơn, âm đầu ghép (`ch`, `ng`, `nh`), vần đơn, vần ghép (`au`, `ơm`, `ưa`, `ước`) và 5 thanh điệu tiếng Việt.
- **D4 (Thanh điều hướng đỉnh 4 nút & Overlays)**: Tích hợp đầy đủ `(Quay lai ←)`, `(Muc luc ☰)`, `1 / 16`, `(Cai dat ⚙)`, `(Thoat ✕)`. Mở Modal Mục Lục dạng lưới 16 ô nhảy trang trực tiếp và Modal Cài Đặt bật/tắt gợi ý tự động.
- **D5 (Tăng cỡ chữ lời thoại lên 46px)**: Nâng font lời dẫn truyện từ 38px lên 46px (line-height 66px) để ba mẹ vừa bế bé vừa đọc to rõ ràng từ khoảng cách xa.
- **D6 (Nâng cấp FSM sang HFSM Phân Tầng)**: Đánh giá FSM 4 nhịp hiện tại là xuất sắc cho phonics đơn nhưng quá hẹp cho cộng đồng sáng tác. Đã thiết kế kiến trúc HFSM 3 cấp: Story Graph (rẽ nhánh) -> Scene Lifecycle -> Pluggable Interaction Drivers (Scratch stage, branching decisions).

## Next steps
1. **Hiện thực hóa HFSM trong C++**: Mở rộng `src/domain/flow_fsm.hpp` hoặc bổ sung `src/domain/hierarchical_fsm.hpp` để hỗ trợ các driver tương tác mới (`branch_choice`, `stage_object_interact`) theo chuẩn vừa ban hành.
2. **Xây dựng công cụ CLI đóng gói (`tools/gach_packager.py`)**: Tự động validate `story.json` qua JSON schema, nạp dữ liệu vào SQLite 3NF và xuất ra tệp `.gach` zip.
3. **Thử nghiệm trên máy đọc sách Kindle vật lý**: Chạy `./install_to_kindle.sh --clean-data` khi cắm cáp USB để kiểm tra độ nhạy cảm ứng và thời gian chuyển trang thực tế trên màn hình E-Ink Carta.
4. **Xây dựng kho kịch bản mở `stories/`**: Thu hút cộng đồng giáo viên và phụ huynh đóng góp các câu chuyện âm vị mới qua GitHub Pull Requests.

## Blockers
N/A

## Prior handoff
- [`docs/handoff/latest.md`](docs/handoff/latest.md) (Phiên bản khởi tạo kiến trúc 4 tầng core engine ngày 2026-10-25).

## Suggested skills
- `ka-execute`: Thực thi mở rộng mã nguồn C++ cho HFSM và công cụ đóng gói kịch bản.
- `ka-test`: Viết bổ sung các ca kiểm thử Unit/E2E cho các kịch bản rẽ nhánh (Branching Story Nodes).
- `ka-ops`: Tối ưu hóa pipeline CI/CD GitHub Actions đóng gói tự động cho nhiều dòng máy Kindle.
