# UI/UX ENGINEERING STANDARD
## Universal Specification for Frontend Quality, User Experience & Interaction Design

---

## 1. TRIẾT LÝ GIAO DIỆN & TRẢI NGHIỆM NGƯỜI DÙNG (CORE PHILOSOPHY)

1. **Tư duy Thấu cảm Người dùng (Cognitive Empathy)**:
   - Một giao diện chạy được code nhưng gây khó hiểu, giật cục hoặc thiếu phản hồi cho người dùng thì **bị coi là thất bại**.
   - Hãy đặt mình vào vị trí của người dùng cuối: Họ không quan tâm backend dùng công nghệ gì; họ quan tâm hệ thống có **nhanh**, **dễ hiểu**, **đáng tin cậy** và **không làm mất dữ liệu của họ** hay không.

2. **Dự đoán được & Nhất quán (Predictability & Consistency)**:
   - Các hành vi tương tác tương tự nhau phải cho ra kết quả tương tự nhau trên toàn bộ ứng dụng.
   - Không được tạo ra các thay đổi giao diện bất ngờ (nhảy layout, giật màn hình - Layout Shift) khi dữ liệu đang nạp.

---

## 2. BẮT BUỘC 4 TRẠNG THÁI GIAO DIỆN (THE 4 MANDATORY UI STATES)

Mọi màn hình, danh sách, form nhập liệu hoặc component hiển thị dữ liệu đều **BẮT BUỘC** phải xử lý trọn vẹn 4 trạng thái sau:

```text
┌────────────────────────────────────────────────────────────────────────┐
│                        THE 4 ESSENTIAL UI STATES                       │
├───────────────────────────────────┬────────────────────────────────────┤
│ 1. EMPTY STATE                    │ 2. LOADING / PENDING STATE         │
│ - Khi danh sách trống / chưa có dữ│ - Khi đang tải dữ liệu hoặc gửi req│
│ - Hiển thị minh họa & thông điệp rõ│ - Dùng Skeleton screens mượt mà    │
│ - Cung cấp nút kêu gọi hành động  │ - Khóa nút (disable) chống submit  │
│   (CTA: "Tạo mục đầu tiên")       │   trùng lặp (double-click)         │
├───────────────────────────────────┼────────────────────────────────────┤
│ 3. ERROR / RECOVERY STATE         │ 4. SUCCESS / ACTIVE STATE          │
│ - Giải thích lỗi bằng ngôn từ dễ  │ - Hiển thị dữ liệu hoàn chỉnh, rõ  │
│   hiểu (tránh mã lỗi thô kỹ thuật)│   ràng thứ bậc thị giác            │
│ - Giữ nguyên dữ liệu form đã gõ   │ - Phản hồi vi mô (micro-feedback): │
│ - Có nút "Thử lại" (Retry action) │   Toast thông báo, animation mượt  │
└───────────────────────────────────┴────────────────────────────────────┘
```

### Chi tiết thực thi:
1. **Empty State**: Không bao giờ để một vùng trắng trơn không lời giải thích. Phải thông báo rõ "Chưa có dữ liệu nào ở đây" kèm theo hướng dẫn hoặc nút bấm để người dùng bắt đầu.
2. **Loading State**: Tránh dùng spinner tròn xoay đơn độc giữa trang nếu có thể dùng **Skeleton Screens**. Skeleton giúp người dùng hình dung trước cấu trúc nội dung sắp xuất hiện, giảm cảm giác chờ đợi.
3. **Error State**: Cung cấp cơ chế tự phục hồi (Recovery path). Người dùng không phải bắt đầu lại từ đầu; form dữ liệu phải được lưu giữ nguyên vẹn.
4. **Success State**: Khi hoàn tất một thao tác quan trọng (lưu thành công, gửi đơn), cần có tín hiệu thị giác xác nhận ngay lập tức (Toast thông báo, tick xanh, chuyển hướng có hiệu ứng).

---

## 3. KỸ THUẬT TƯƠNG TÁC & PHẢN HỒI THỜI GIAN THỰC (INTERACTION ENGINEERING)

### 3.1. OPTIMISTIC UPDATES (CẬP NHẬT GIAO DIỆN LẠC QUAN)
- Với các thao tác tương tác thường nhật (như nhấn Thích, Đánh dấu hoàn thành, Đổi trạng thái checkbox):
  * Cập nhật giao diện ngay lập tức trong vòng $16\text{ms}$ (1 frame) trước khi nhận được phản hồi từ server.
  * Nếu server trả về lỗi: Tự động khôi phục (Rollback) lại trạng thái cũ kèm theo thông báo nổi giải thích lý do lỗi.

### 3.2. DEFENSIVE UI CONTROLS (PHÒNG THỦ TƯƠNG TÁC)
1. **Chống bấm liên tục (Double-Click / Rapid Submission Protection)**:
   - Mọi nút bấm gửi dữ liệu (`Submit`, `Thanh toán`, `Lưu`, `Xóa`) phải lập tức chuyển sang trạng thái `disabled` và hiển thị chỉ báo đang xử lý (spinner nhỏ hoặc icon loading) ngay khi người dùng click lần đầu.
2. **Debounce Tìm kiếm & Input**:
   - Các ô input tìm kiếm thời gian thực phải được áp dụng `debounce` (tối thiểu $250\text{ms} - 350\text{ms}$) trước khi phát sinh request mạng hoặc lọc dữ liệu nặng.
3. **Throttle Sự kiện Tần số Cao**:
   - Các sự kiện `scroll`, `resize`, `mousemove` phải được `throttle` hoặc sử dụng `requestAnimationFrame` để tránh làm đơ giao diện (UI jank).

---

## 4. KHẢ NĂNG TIẾP CẬN (ACCESSIBILITY - A11Y) & BÀN PHÍM

1. **Điều hướng bằng Bàn phím (Keyboard Navigation)**:
   - Toàn bộ các tương tác trên trang phải có thể thực hiện được bằng bàn phím mà không cần chuột:
     * Phím `Tab` di chuyển tuần tự giữa các thành phần tương tác.
     * Phím `Enter` / `Space` kích hoạt nút bấm hoặc liên kết.
     * Phím `Escape` đóng nhanh các modal popup, dropdown menu hoặc drawer.
     * Các phím mũi tên (`Arrow keys`) điều hướng trong danh sách hoặc tabs.
2. **Bẫy tiêu điểm trong Modal (Focus Trapping)**:
   - Khi một Modal Popup hoặc Dialog mở ra:
     * Tiêu điểm (Focus) phải tự động chuyển vào phần tử đầu tiên bên trong Modal.
     * Khi người dùng nhấn `Tab`, tiêu điểm chỉ được di chuyển tuần hoàn bên trong Modal, không được nhảy ra ngoài trang nền.
     * Khi Modal đóng lại: Trả tiêu điểm về đúng nút đã kích hoạt Modal trước đó.
3. **HTML Semantic & Trợ năng**:
   - Sử dụng đúng thẻ semantic: `<button>` cho hành động, `<a>` cho chuyển hướng trang, `<nav>`, `<main>`, `<dialog>`.
   - Cung cấp `aria-label` cho các nút bấm chỉ có icon (icon-only buttons).
   - Đảm bảo độ tương phản màu sắc đạt chuẩn tối thiểu **WCAG AA** (tỷ lệ tương phản $4.5:1$ đối với văn bản thông thường).

---

## 5. BẢO VỆ TÀI NGUYÊN & VÒNG ĐỜI TRONG GIAO DIỆN (UI RESOURCE LIFECYCLE)

Để ngăn chặn rò rỉ bộ nhớ (Memory Leaks) và suy giảm hiệu năng trình duyệt theo thời gian:

1. **Thu hồi Event Listeners & Timers**:
   - Mọi `window.addEventListener` hoặc `document.addEventListener` phải có hàm gỡ bỏ (`removeEventListener`) khi component unmount.
   - Mọi `setTimeout` hoặc `setInterval` phải được hủy (`clearTimeout`, `clearInterval`).
2. **Hủy yêu cầu mạng đang dở dang (Abort In-Flight Requests)**:
   - Sử dụng `AbortController` để hủy bỏ các request fetch/axios khi component unmount hoặc khi người dùng nhập từ khóa mới trước khi request cũ kịp trả về.
3. **Giải phóng Media & Object URLs**:
   - Mọi `URL.createObjectURL(blob)` phục vụ xem trước ảnh/video/audio phải được thu hồi qua `URL.revokeObjectURL(url)` khi ảnh bị xóa, thay thế hoặc khi đóng component.
   - Các luồng MediaStream từ Camera/Microphone phải được dừng tường minh (`track.stop()`).
4. **Tránh Re-renders vô ích & Giữ FPS 60**:
   - Tránh khởi tạo object/function mới trong render loop nếu không cần thiết.
   - Giữ DOM nhỏ gọn, áp dụng Virtualization (danh sách ảo) khi render danh sách trên 100 phần tử.

---

## 6. UI/UX ENGINEERING CHECKLIST

Trước khi nghiệm thu một giao diện:
- [ ] Đã kiểm thử và xử lý cả 4 trạng thái: Empty, Loading, Error và Success.
- [ ] Không có layout shift giật cục khi dữ liệu tải xong.
- [ ] Nút submit bị khóa ngay lập tức khi người dùng click để chống gửi trùng lặp.
- [ ] Có thể điều hướng toàn bộ màn hình và đóng modal bằng phím `Tab`, `Enter`, `Escape`.
- [ ] Toàn bộ Event listeners, Intervals, AbortControllers và Object URLs đều được cleanup an toàn khi unmount.
- [ ] Thông báo lỗi thân thiện với người dùng, không hiển thị stack trace kỹ thuật trần trụi.
---

## 7. PHỤ LỤC EMBEDDED C++: GIAO DIỆN & TRẢI NGHIỆM E-INK CHO TRẺ 3–6 TUỔI (GACH ENGINE)

### 7.1. BỐN TRẠNG THÁI GIAO DIỆN TRÊN MÀN HÌNH MỰC ĐIỆN TỬ (E-INK)
1. **Empty State**: Khi chưa có sách hoặc chưa nạp được trang $ightarrow$ Hiển thị hình vẽ nét mực bạn Mít đứng vẫy tay kèm thông điệp mộc mạc: "Bé chọn cuốn sách để cùng Mít đọc nhé".
2. **Loading State**: Trong $< 15	ext{ ms}$ nạp ảnh từ flash $ightarrow$ Giữ nguyên khung viền tĩnh, tuyệt đối không dùng spinner xoay tròn liên tục (gây chớp nháy làm hỏng trải nghiệm thị giác E-Ink).
3. **Error State**: Nếu gặp lỗi tệp ảnh hoặc database $ightarrow$ Thông báo yêu thương: "Cuốn sách đang nghỉ ngơi một chút, cha mẹ giúp bé mở lại nhé".
4. **Success State**: Khi bé chạm đúng mảnh ghép $ightarrow$ Tranh cảnh quả ngọt bung nở, viền từ ghép chuyển từ nét đứt sang nét đậm hoàn chỉnh, khoảng lặng êm đềm xuất hiện.

### 7.2. KỶ LUẬT LÀM MỚI MÀN HÌNH E-INK (E-INK REFRESH DISCIPLINE)
- **Hạn chế tối đa chớp toàn màn hình (Full Flash)**:
  * Trẻ nhỏ rất dễ bị phân tâm và mỏi mắt nếu màn hình đảo màu đen-trắng liên tục.
  * **Chỉ Full Refresh** khi: Sang trang mới hoặc bước vào nhịp **TRANSFORM** (chuyển sang cảnh quả ngọt) để xóa sạch bóng mờ (ghosting).
  * **Áp dụng Partial Refresh (Dirty Rectangle)** khi: Bé chạm vào các mảnh vần ở Nhịp 2 hoặc khi hiện viền trợ giúp sau 6s. Chỉ vẽ lại đúng vùng chữ nhật thay đổi qua `cairo_region_t`.

### 7.3. CÔNG THỨC HITBOX & TƯƠNG TÁC TAY BÉ TRÊN MÀN CẢM ỨNG KINDLE
- **Kích thước Hitbox tối thiểu**:
  * Trên Kindle PW3 ($1448 	imes 1072	ext{ px}$): Hitbox mảnh ghép tối thiểu **$120 	imes 120	ext{ px}$**.
  * Trên Kindle PW2 ($1024 	imes 758	ext{ px}$): Hitbox mảnh ghép tối thiểu **$80 	imes 80	ext{ px}$**.
  * Khoảng cách an toàn giữa 2 hitbox: Tối thiểu $24	ext{ px}$ để chống chạm nhầm ngón tay.
- **Phòng thủ chạm liên tục (Rapid-Tap Guard)**:
  * Khóa cảm ứng hoàn toàn trong Nhịp 1 (**OBSERVE** 2.5s) và Nhịp 3 (**TRANSFORM**).
  * Bỏ qua mọi sự kiện chạm lặp lại trong vòng $300	ext{ ms}$ (Debounce chạm vật lý) để tránh nhận diện nhầm do độ trễ cảm ứng của màn E-Ink.

### 7.4. TRỰC QUAN HÓA ÂM VỊ (VISUAL PHONICS) KHI KHÔNG CÓ LOA
- Vì Kindle PW2/PW3 **hoàn toàn không có loa ngoài và jack 3.5mm**:
  * Âm thanh được chuyển hóa thành **Ngôn ngữ Thị giác (Visual Phonics)**:
    - Vần khuyết được biểu thị bằng ô chữ nhật viền nét đứt (dashed border) nổi bật.
    - Mảnh vần lựa chọn được in to, rõ ràng, kiểu chữ chân phương dễ đọc cho trẻ mầm non.
    - Khi ghép đúng, mảnh vần trượt nhẹ vào vị trí và nối liền mạch thành từ hoàn chỉnh với nét mực đậm đặc trưng của E-Ink.
  * Môi trường đọc sách thúc đẩy sự kết nối gia đình: Cha mẹ ngồi cạnh, phát âm tự nhiên âm vị cùng con.
