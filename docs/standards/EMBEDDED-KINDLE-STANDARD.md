# TIÊU CHUẨN KỸ NGHỆ EMBEDDED C++ CHO MÁY ĐỌC SÁCH KINDLE PW2/PW3
## Ràng Buộc Phần Cứng, Tối Ưu Tài Nguyên & Hiệu Suất Cao Cốt Tử (GACH Engine)

---

## 1. RÀNG BUỘC PHẦN CỨNG & HẠ TẦNG VẬT LÝ

| Thành phần | Kindle Paperwhite 2 (PW2 - 2013) | Kindle Paperwhite 3 (PW3 - 2015) | Ràng buộc kỹ thuật đối với GACH |
| :--- | :--- | :--- | :--- |
| **CPU** | Freescale i.MX6 SoloLite (Cortex-A9, 1.0 GHz, ARMv7 32-bit) | Tương tự (1 nhân Cortex-A9, ARMv7 hard-float) | Không tính toán nặng runtime; thuật ngữ toán học/thuật toán phải là $O(1)$ hoặc $O(N)$ rất nhỏ. |
| **RAM** | Tổng 512MB (Hệ thống Amazon chiếm ~470MB) | Tương tự (Khả dụng cho app người dùng: **~35 - 45MB**) | **Khóa cứng trần RAM toàn ứng dụng (RSS) < 12MB**. Không được phép vượt quá. |
| **Màn hình E-Ink** | E-Ink Pearl, $1024 	imes 758	ext{ px}$ (212 PPI, 16 mức xám) | E-Ink Carta, $1448 	imes 1072	ext{ px}$ (300 PPI, 16 mức xám) | Độ trễ hiển thị cao (~200-450ms). Bắt buộc dùng **Dirty Rect Partial Refresh**. |
| **Cảm ứng** | Điện dung hồng ngoại/cảm ứng 2 điểm | Cảm ứng điện dung bề mặt | Tránh cử chỉ phức tạp (swipe/pinch). Chỉ dùng **Tap/Touch đơn điểm**. Hitbox tối thiểu $ge 80 	imes 80	ext{ px}$ (PW2), $ge 120 	imes 120	ext{ px}$ (PW3). |
| **Âm thanh** | **Hoàn toàn KHÔNG có loa và jack 3.5mm** | **Hoàn toàn KHÔNG có loa và jack 3.5mm** | **Trực quan hóa âm vị (Visual Phonics)**; cha mẹ phát âm đồng hành cùng bé. |
| **Lưu trữ** | Phân vùng người dùng `/mnt/us` định dạng **FAT32** | `/mnt/us` định dạng **FAT32** | Phân vùng FAT32 không hỗ trợ POSIX permissions: **Kiểm tra file bắt buộc dùng `access(F_OK)`, CẤM dùng `access(X_OK)`**. |
| **Nguồn điện & Pin** | Pin Li-ion ~1420 mAh | Pin Li-ion ~1420 mAh | Tiết kiệm pin tối đa: Không chạy vòng lặp bận (busy loop); ngủ khi idle trong GTK event loop; hook LIPC ngăn OS sleep giữa chừng. |

---

## 2. KỶ LUẬT QUẢN TRỊ BỘ NHỚ (ZERO-DYNAMIC-ALLOCATION IN HOT PATH)

1. **Khóa cứng trần RAM toàn bộ App < 12MB**:
   - Bộ đệm khung hình Cairo + GTK: $approx 2.5	ext{ MB}$.
   - Bộ đệm 1 ảnh minh họa Grayscale PNG: $approx 1.1	ext{ MB}$.
   - SQLite3 Cache & Internal structs: $approx 500	ext{ KB}$ (cấu hình cứng `PRAGMA cache_size = -500;`).
   - Font cache & FreeType glyphs: $approx 1.5	ext{ MB}$.
   - Stack + Text segment + Data: $approx 2.5	ext{ MB}$.
   - $ightarrow$ **Tổng RAM thường trú (RSS) dự kiến: $sim 7.6	ext{ MB} ll 12	ext{ MB}$ (Đạt biên an toàn cực cao)**.

2. **Cấm cấp phát Heap trong Hot Path (Zero-Dynamic-Allocation)**:
   - Trong luồng vẽ (`CairoRenderer::render_beat`) và luồng xử lý cảm ứng (`GtkTouchAdapter::handle_touch`):
     * **TUYỆT ĐỐI CẤM** dùng `new`, `delete`, `malloc`, `free`.
     * **TUYỆT ĐỐI CẤM** dùng các thao tác tái phân bổ bộ nhớ động như `std::vector::push_back` vượt capacity, nối chuỗi `std::string += ...` liên tục trong loop vẽ.
     * Mọi cấu trúc dữ liệu âm vị, tọa độ hitbox, bounding boxes phải được cấp phát trước (`std::array`, fixed buffers) hoặc nằm hoàn toàn trên **Stack**.

3. **Value Objects bất biến trên Stack**:
   - Kích thước của `PhonicsSyllable`, `StoryBeat`, `TouchHitbox` phải được khống chế $le 64	ext{ bytes}$ để truyền nhận theo giá trị (Pass-by-value) hoặc const reference (`const &`) mà không tốn công cấp phát con trỏ.
   - Tận dụng triệt để `std::string_view` cho các chuỗi ký tự UTF-8 bất biến từ bộ nhớ tĩnh hoặc database buffer.

---

## 3. QUY CHUẨN TÀI NGUYÊN HÌNH ẢNH & FONT CHO MÀN HÌNH E-INK

1. **Định dạng ảnh Grayscale 1-channel (8-bit)**:
   - Tuyệt đối không render SVG trực tiếp trên Kindle (CPU đơn nhân 1.0 GHz sẽ bị đơ/lag 3–5 giây khi phân tích vector).
   - Toàn bộ tranh minh họa được lưu trữ gốc ở định dạng SVG trên máy phát triển, sau đó được build script chuyển đổi sang **PNG Grayscale 1-channel (8-bit hoặc 4-bit 16 mức xám)** với kích thước vừa khít khung tranh vật lý:
     * Kích thước khung tranh trên PW3: $1072 	imes 1014	ext{ px}$.
     * Kích thước khung tranh trên PW2: $758 	imes 716	ext{ px}$.
   - Tốc độ nạp ảnh từ flash FAT32 vào bộ đệm Cairo: $< 15	ext{ ms}$.

2. **Cơ chế tái tạo màn hình E-Ink (E-Ink Refresh Discipline)**:
   - Màn hình E-Ink chịu hiện tượng lưu ảnh (Ghosting) và chớp đảo màu (Flashing).
   - Với trẻ 3–6 tuổi, chớp màn hình liên tục sẽ gây mất tập trung và khó chịu cho mắt bé.
   - **Quy tắc làm mới 2 cấp độ**:
     * **Nhịp 1, 2, 4 (Tương tác nhỏ, chạm chọn mảnh vần, hiện mũi tên)**: Áp dụng **Partial Refresh (Dirty Rectangle)** chỉ tại khu vực mảnh vần / hitbox, không chớp toàn màn hình.
     * **Nhịp 3 (TRANSFORM - Chuyển sang tranh quả ngọt) hoặc Sang trang mới**: Kích hoạt **Full Screen Flash** một lần duy nhất để xóa sạch hoàn toàn ghosting và hiển thị bức tranh quả ngọt rõ nét nhất.

---

## 4. QUY CHUẨN CƠ SỞ DỮ LIỆU SQLITE3 TRÊN HỆ THỐNG FAT32

1. **Chuẩn hóa 3NF và Toàn vẹn Tham chiếu**:
   - Cơ sở dữ liệu `story_engine.db` được thiết kế theo chuẩn 3NF: các bảng `stories`, `pages`, `phonics_targets`, `choices`.
   - Bắt buộc kích hoạt khóa ngoại cứng: `PRAGMA foreign_keys = ON;`.

2. **Tối ưu hóa Flash FAT32 & Chống phân mảnh**:
   - Phân vùng `/mnt/us` có tốc độ ghi flash hạn chế và dễ phân mảnh nếu ghi nhiều journal files.
   - Thiết lập cấu hình SQLite bắt buộc khi mở kết nối:
     * `PRAGMA cache_size = -500;`: Khóa cứng bộ đệm RAM ở mức đúng 500KB.
     * `PRAGMA journal_mode = DELETE;`: Tránh để lại các file `-wal` và `-shm` trên phân vùng FAT32 của Kindle.
     * `PRAGMA synchronous = NORMAL;`: Giảm số lần gọi `fsync()` xuống mức an toàn mà vẫn bảo toàn cơ sở dữ liệu khi tắt máy đột ngột.
     * `PRAGMA temp_store = MEMORY;`: Không ghi file tạm xuống flash memory của Kindle.

---

## 5. BẢO VỆ NGUỒN ĐIỆN & QUẢN LÝ TIẾN TRÌNH TRÊN KINDLE OS

1. **Khởi chạy qua KUAL (Kindle Unified Application Launcher)**:
   - Script khởi chạy `extension/gach/bin/start.sh` kiểm tra binary qua `access(F_OK)`.
   - Ngừng tạm thời trình đọc sách mặc định của Amazon (`lipc-set-prop com.lab126.appmgrd stop app://com.lab126.booklet.home` hoặc tương đương) để giải phóng RAM tối đa cho GACH.
   - Khi thoát app: Khôi phục lại trạng thái hệ thống Kindle mượt mà, không làm treo máy.

2. **Chống tự động khóa máy (Prevent Kindle Sleep)**:
   - Gửi tín hiệu giả lập hoạt động qua LIPC (`lipc-set-prop com.lab126.powerd -i touchScreenEvent 1`) theo chu kỳ nhịp truyện để màn hình không bị khóa khi bé đang ngồi ngẫm nghĩ hoặc nghe cha mẹ đọc truyện.
