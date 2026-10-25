# GACH — Kindle-Mít Story Phonics Engine

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![Architecture](https://img.shields.io/badge/Architecture-Clean%204--Tier-success.svg)](docs/standards/ARCHITECTURE.md)
[![Hardware](https://img.shields.io/badge/Hardware-Kindle%20PW2%2FPW3-orange.svg)](docs/standards/EMBEDDED-KINDLE-STANDARD.md)
[![Tests](https://img.shields.io/badge/Tests-100%25%20Passing%20(ASan%20Clean)-brightgreen.svg)](tests/)

> **GACH** (Kindle-Mít) là Story Phonics & Moral Agency Engine viết bằng **C++17 thuần túy (Clean Architecture 4 tầng)** dành riêng cho máy đọc sách **Kindle Paperwhite 2/3 (PW2/PW3)**.
> Dự án kết hợp phương pháp ghép vần tiếng Việt tự nhiên với những câu chuyện đời thường nuôi dưỡng lòng trắc ẩn của bạn Mít.

---

## 1. TRIẾT LÝ SƯ PHẠM & ĐỊNH DANH GACH

### 1.1. Backronym Định Danh & Trục Nhân Duyên
- **G**iác quan (Mắt thấy nét mực E-Ink thanh tịnh, tay chạm màn hình - Sắc/Thọ) $leftrightarrow$ **G**ốc (Nhân)
- **Â**m vị (Nhận diện thanh âm, ghép vần Blending - Tưởng) $leftrightarrow$ **Â**m (Lời nói ái ngữ)
- **C**ảm nhận (Thấu cảm hoàn cảnh, khơi gợi lòng trắc ẩn - Thức) $leftrightarrow$ **C**ảnh (Duyên lành)
- **H**ành động (Chạm mở khóa điều tử tế - Hành) $leftrightarrow$ **H**ạnh (Quả ngọt an vui)

### 1.2. Triết Lý Anti-Dopamine
- **Không điểm số, không sao vàng, không bảng xếp hạng, không âm thanh kích động**.
- Trẻ mầm non 3–6 tuổi được học trong bầu không khí tĩnh tại, thuần khiết.
- Khi chọn sai: Giữ nguyên trạng thái bình yên, không phạt, không rung chuông báo lỗi, cho phép bé thử lại tự nhiên.

---

## 2. RÀNG BUỘC PHẦN CỨNG & KỶ LUẬT TỐI ƯU HÓA E-INK

| Thành phần | Đặc tính phần cứng Kindle PW2/PW3 | Giải pháp kỹ thuật trong GACH Engine |
| :--- | :--- | :--- |
| **CPU** | Freescale i.MX6 SoloLite (Cortex-A9, 1.0 GHz, ARMv7) | Thuật toán $O(1)$, không tính toán nặng tại runtime. |
| **RAM** | Tổng 512MB, khả dụng cho app chỉ **~35-45MB** | **Khóa cứng trần RAM toàn app < 12MB** (RSS đo được: $sim 7.6	ext{ MB}$). |
| **Bộ nhớ nóng** | Luồng vẽ Cairo & Bắt cảm ứng | **Zero-Dynamic-Allocation**: Cấm `new`/`malloc` trong hot-path. Value Objects nằm trên Stack. |
| **Màn hình E-Ink** | Carta/Pearl 16 mức xám, độ trễ làm mới cao | **Partial Refresh (Dirty Rectangle)** khi tương tác nhỏ; **Full Refresh** khi chuyển cảnh quả ngọt. |
| **Âm thanh** | **Hoàn toàn KHÔNG có loa ngoài & jack 3.5mm** | **Trực quan hóa âm vị (Visual Phonics)**; cha mẹ phát âm đồng hành cùng con. |
| **Hệ thống tệp** | Phân vùng `/mnt/us` định dạng **FAT32** | Kiểm tra tệp bằng **`access(F_OK)`**, tuyệt đối cấm dùng `access(X_OK)`. |
| **Cơ sở dữ liệu** | Flash memory FAT32 | **SQLite3 3NF** (`story_engine.db`), cache khóa cứng **500KB** (`cache_size = -500`), `journal_mode = DELETE`. |

---

## 3. BẢN ĐỒ KIẾN TRÚC CLEAN ARCHITECTURE 4 TẦNG

```text
gach-kindle/
├── docs/                           # Tài liệu kỹ thuật, tiêu chuẩn AI, đặc tả FSM, Schema 3NF
│   ├── standards/                  # Bộ 7 tiêu chuẩn kỹ nghệ phần mềm nhúng
│   └── architecture/               # FSM 4 nhịp & lược đồ CSDL
├── data/                           # Cơ sở dữ liệu SQLite3 & Kịch bản mẫu
│   ├── schema.sql                  # DDL 3NF (stories, pages, phonics_targets, choices)
│   └── seed_story_mit.sql          # Kịch bản mẫu: "Mít tưới cây cho bà" (3 trang)
├── assets/images/                  # Tranh minh họa Grayscale PNG 1-channel (1072x1014)
├── src/
│   ├── domain/                     # [TẦNG 1: PURE DOMAIN] Pure C++17, Zero IO, Stack-first
│   │   ├── phonics_syllable.hpp    # Value Object âm vị (Onset, Rhyme, Tone, FullWord) <= 64 bytes
│   │   ├── flow_fsm.hpp            # FSM 4 nhịp (OBSERVE -> AWAIT_INPUT -> TRANSFORM -> COMPLETED)
│   │   ├── story_beat.hpp          # Value Object nhịp truyện & mảnh ghép
│   │   └── domain_errors.hpp       # Domain exceptions & Invariant guards
│   ├── application/                # [TẦNG 2: USE CASES & PORTS]
│   │   ├── ports/                  # Inbound (IUserInputPort) & Outbound Ports (IStoryRepository, IDisplayRenderer)
│   │   └── story_interactor.hpp    # Điều phối dòng chảy đọc truyện, timers, và tương tác
│   ├── adapters/                   # [TẦNG 3: ADAPTERS]
│   │   ├── db/                     # SqliteStoryRepository (SQLite3 C-API, cache 500KB)
│   │   └── ui/                     # CairoRenderer, UILayout (hitbox >= 120px), GtkTouchAdapter
│   └── infrastructure/             # [TẦNG 4: INFRASTRUCTURE]
│       ├── app_config.hpp          # Cấu hình tham số độ phân giải, đường dẫn DB
│       ├── kindle_power.hpp        # Hook LIPC ngăn Kindle sleep, POSIX signals
│       └── main.cpp                # Dependency Injection Bootstrap & Entry Point
├── tests/                          # Kim tự tháp kiểm thử 70/20/10
│   ├── unit/                       # Unit tests: PhonicsSyllable, FlowFSM (< 5ms)
│   ├── integration/                # Integration tests: SqliteStoryRepository (:memory:, 0.04ms/query)
│   └── e2e/                        # End-to-End test: Giả lập trọn vẹn luồng đọc truyện 3 trang
├── extension/                      # KUAL extension package (config.xml, menu.json, bin/start.sh)
├── scripts/                        # package-kual.sh, build-kindlehf.sh, generate_images.cpp
├── Makefile                        # Build pipeline tự động hóa
└── story_engine.db                 # CSDL SQLite3 3NF tạo sẵn
```

---

## 4. HƯỚNG DẪN BIÊN DỊCH & KIỂM THỬ

### 4.1. Chạy Toàn Bộ Test Suite (TDD Verification)
```bash
# Chạy cả 3 tầng: Unit tests, SQLite Integration tests, E2E Workflow tests
make test
```

### 4.2. Thẩm Định Rò Rỉ Bộ Nhớ (AddressSanitizer Audit)
```bash
# Kiểm tra 0 memory leaks & 0 undefined behavior trên toàn bộ codebase
make test_asan
```

### 4.3. Biên Dịch Ứng Dụng Chạy Trên Máy Tính (Host GTK3/Cairo)
```bash
make bin/gach

# Chạy thử nghiệm ở độ phân giải mô phỏng máy tính (600x850)
./bin/gach --desktop --db story_engine.db

# Hoặc mô phỏng chuẩn màn hình Kindle PW3 (1072x1448)
./bin/gach --pw3 --db story_engine.db
```

### 4.4. Đóng Gói KUAL Cài Đặt Lên Kindle PW2/PW3
```bash
./scripts/package-kual.sh
```
Kết quả sẽ tạo ra file nén siêu nhẹ **`gach-kual.zip`** ($sim 204	ext{ KB}$).

---

## 5. CÀI ĐẶT LÊN KINDLE PAPERWHITE

1. Kết nối Kindle với máy tính qua cáp USB.
2. Giải nén toàn bộ nội dung của tệp `gach-kual.zip` vào thư mục:
   ```text
   /mnt/us/extensions/
   ```
   (Đường dẫn thư mục sau khi giải nén sẽ là `/mnt/us/extensions/gach/`).
3. Ngắt kết nối USB an toàn.
4. Mở menu **KUAL** trên màn hình Kindle và chạm vào mục **GACH Story Phonics (Bé Mít)** để bắt đầu!


---

## 6. QUY TRÌNH BIÊN DỊCH CHÉO ARMV7 QUA GITHUB ACTIONS

Vì máy tính phát triển sử dụng kiến trúc **x86_64**, trong khi Kindle PW2/PW3 sử dụng vi xử lý **Freescale i.MX6 SoloLite (Cortex-A9, ARMv7 32-bit)** với phiên bản glibc đặc thù của Amazon, việc biên dịch binary chạy trực tiếp trên thiết bị được tự động hóa hoàn toàn qua **GitHub Actions CI** (`.github/workflows/build-kindle.yml`):

1. **Toolchain Chính thức**: Sử dụng **KOReader koxtoolchain (`kindlepw2.tar.gz`)** và **Kindle SDK (`kindle-sdk`)** để bảo đảm binary tương thích 100% với hệ thống tệp và glibc của Kindle OS.
2. **Tự động Đóng gói & Phát hành**: Mỗi khi bạn push code lên GitHub (`main`/`master` hoặc tag `v*`), GitHub Actions sẽ:
   - Biên dịch chéo sang ARMv7 bằng Meson (`meson setup --cross-file ...`).
   - Tối ưu kích thước bằng `strip` (`-static-libstdc++ -static-libgcc`).
   - Đóng gói tài nguyên, hình ảnh và SQLite database vào `gach-kual.zip`.
   - Tạo bản phát hành tự động tại **GitHub Releases**.
3. **Cài đặt Tự động từ Xa**: Script `install_to_kindle.sh` sẽ tự động nhận diện nếu binary trên máy là x86_64 để tải ngay bản ARMv7 mới nhất từ GitHub Releases nạp vào Kindle.

---

## 7. KẾT QUẢ NGHIỆM THU CHẤT LƯỢNG

- **Unit Tests (Domain)**: 100% PASS (thời gian thực thi: $< 4	ext{ ms}$).
- **Integration Tests (SQLite 3NF)**: 100% PASS (1000 truy vấn hoàn thành trong $41.2	ext{ ms}$, trung bình **$0.041	ext{ ms/truy vấn}$**).
- **E2E Workflow Tests**: 100% PASS (14 nhịp render hoàn chỉnh qua 3 trang truyện).
- **AddressSanitizer & UndefinedBehaviorSanitizer**: **0 LEAKS, 0 ERRORS**.
- **Kích thước file thực thi**: Binary `bin/gach` chỉ **$81	ext{ KB}$**, gói KUAL zip chỉ **$204	ext{ KB}$**.
