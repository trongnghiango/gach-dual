# ĐẶC TẢ MÁY TRẠNG THÁI DÒNG CHẢY 4 NHỊP (UNIFIED 4-BEAT FLOW FSM)

## 1. MỤC TIÊU THIẾT KẾ
Thay vì xây dựng các máy trạng thái rời rạc cho từng tập truyện, **GACH Engine** sử dụng một **FSM 4 nhịp duy nhất** điều khiển tất cả các trang truyện theo quy luật tự nhiên và sư phạm Anti-Dopamine:

```text
┌─────────────────┐       2.5s timeout       ┌─────────────────┐
│ 1. OBSERVE      │ ───────────────────────► │ 2. AWAIT_INPUT  │
│ (Khóa cảm ứng,  │                          │ (Hiện mảnh vần, │
│ ngắm tranh tĩnh)│                          │ bé chạm chọn)   │
└─────────────────┘                          └────────┬────────┘
                                                      │
                                                      │ Chạm đúng vần
                                                      ▼
┌─────────────────┐     Khoảng lặng êm đềm   ┌─────────────────┐
│ 4. COMPLETED    │ ◄─────────────────────── │ 3. TRANSFORM    │
│ (Hiện mũi tên,  │                          │ (Tranh quả ngọt,│
│ bé lật trang)   │                          │ từ vựng trọn vẹn│
└────────┬────────┘                          └─────────────────┘
         │
         │ Chạm nút Next
         ▼
[Trang Mới: OBSERVE]
```

---

## 2. CHI TIẾT 4 NHỊP TRẢI NGHIỆM

### Nhịp 1: OBSERVE (Quan Sát Tĩnh & Lắng Nghe - 2.5 giây)
- **Hành vi**: Màn hình hiển thị bức tranh mở đầu cùng lời dẫn truyện mộc mạc.
- **Guard**: Cảm ứng bị khóa hoàn toàn (`is_touch_locked() == true`).
- **Ý nghĩa sư phạm**: Giúp bé 3–6 tuổi dừng lại ngắm nhìn cảnh vật, hòa mình vào không gian của bạn Mít, lắng nghe cha mẹ đọc lời dẫn mà không bị thói quen "bấm loạn" màn hình.

### Nhịp 2: AWAIT_INPUT (Tương Tác Ghép Vần)
- **Hành vi**: Xuất hiện từ vựng với vần khuyết (viền nét đứt) và các mảnh ghép lựa chọn (to, rõ, dễ chạm).
- **Phản hồi khi chạm sai**: Giữ nguyên trạng thái, không trừ điểm, không âm thanh báo lỗi, bé có thể thử lại tự nhiên.
- **Tự động dẫn lối (Subtle Guide)**: Sau 6.0 giây nếu bé chưa chạm, viền của mảnh vần đúng sẽ nhẹ nhàng đậm lên để hướng dẫn bé mà không tạo áp lực.

### Nhịp 3: TRANSFORM (Chuyển Hóa Tử Tế & Khoảng Lặng)
- **Hành vi**: Khi bé ghép đúng mảnh vần, bức tranh lập tức chuyển sang cảnh quả ngọt (cây xanh mướt mỉm cười, bà xoa đầu khen Mít ngoan).
- **Ý nghĩa sư phạm**: Hành động tử tế lập tức mang lại kết quả an vui (Nhân $ightarrow$ Duyên $ightarrow$ Quả). Khoảng lặng vài giây giúp bé thấu cảm sâu sắc.

### Nhịp 4: COMPLETED (Chủ Động Sang Trang)
- **Hành vi**: Nút mũi tên chuyển trang xuất hiện ở góc phải màn hình.
- **Ý nghĩa sư phạm**: Bé là người làm chủ nhịp đọc, tự tay chạm để bước sang trang tiếp theo khi đã sẵn sàng.
