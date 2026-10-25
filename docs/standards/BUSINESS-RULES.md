# BUSINESS RULES & DOMAIN MODELING STANDARD
## Universal Specification for Domain Logic, State Machines & Business Invariants

---

## 1. NGUYÊN TẮC CỐT LÕI: BUSINESS LOGIC LÀ FIRST-CLASS CITIZEN

1. **Vị trí của Nghiệp vụ (Logic Placement)**:
   - Nghiệp vụ là tài sản quan trọng nhất của toàn bộ hệ thống phần mềm.
   - **Tuyệt đối cấm**:
     * Đặt business logic trong UI components, React hooks hoặc CSS helpers.
     * Đặt business logic trong HTTP Controllers, Express middleware hoặc API routers.
     * Đặt business logic ngầm trong Database Triggers, Stored Procedures hoặc SQL raw queries nếu không có sự quản lý của Domain Layer.
   - **Bắt buộc**: Toàn bộ quy tắc tính toán, điều kiện ràng buộc, thẩm định nghiệp vụ phải tập trung 100% trong **Domain Layer** và **Application Use Cases**.

2. **Ngôn ngữ Nghiệp vụ Thống nhất (Ubiquitous Language)**:
   - Tên biến, hàm, class, enum, method trong code phải phản ánh chính xác thuật ngữ của nghiệp vụ thực tế.
   - Cấm đặt tên mơ hồ theo kiểu kỹ thuật chung chung (`data`, `info`, `itemObj`, `handleProcess`, `doStuff`).
   - Phải sử dụng từ ngữ mang tính hành động và phản ánh bản chất nghiệp vụ (`cancelPendingOrder`, `calculateTaxExemption`, `expireTrialSubscription`, `reserveInventoryQuota`).

---

## 2. MÔ HÌNH HÓA DOMAIN: ENTITIES & VALUE OBJECTS

### 2.1. RICH DOMAIN MODEL (MÔ HÌNH MIỀN GIÀU CÓ)
- Tuyệt đối tránh **Anemic Domain Model** (đối tượng chỉ gồm các trường dữ liệu và getter/setter).
- Domain Entity phải tự chịu trách nhiệm bảo vệ tính toàn vẹn (Invariants) của chính nó thông qua các phương thức hành vi:
  ```typescript
  // ❌ SAI: Anemic Model - Cho phép code bên ngoài tự do gán dữ liệu phá vỡ quy tắc
  class Order {
    public status: string;
    public totalAmount: number;
  }
  // Ở Controller:
  order.status = "COMPLETED"; // Không kiểm tra điều kiện thanh toán!

  // ✅ ĐÚNG: Rich Domain Model - Phương thức nghiệp vụ tự bảo vệ invariants
  class Order {
    private _status: OrderStatus;
    private _isPaid: boolean;

    public complete(): Result<void, DomainError> {
      if (!this._isPaid) {
        return Result.fail(new DomainError("ORDER_UNPAID", "Không thể hoàn tất đơn hàng chưa thanh toán"));
      }
      if (this._status !== OrderStatus.PROCESSING) {
        return Result.fail(new DomainError("INVALID_TRANSITION", "Đơn hàng chỉ được hoàn tất từ trạng thái Đang xử lý"));
      }
      this._status = OrderStatus.COMPLETED;
      this.addDomainEvent(new OrderCompletedEvent(this.id));
      return Result.ok();
    }
  }
  ```

### 2.2. VALUE OBJECTS (ĐỐI TƯỢNG GIÁ TRỊ BẤT BIẾN)
- **Đặc tính**: Không có Identity độc lập; được định nghĩa hoàn toàn bởi giá trị; **bất biến (Immutable)**.
- **Tự thẩm định tính hợp lệ ngay khi khởi tạo (Self-Validating)**: Không cho phép tạo ra một Value Object chứa giá trị không hợp lệ.
- **Ví dụ**: `Money`, `EmailAddress`, `DateRange`, `PhoneNumber`, `PercentageDiscount`.
  ```typescript
  export class Money {
    private constructor(public readonly amount: number, public readonly currency: Currency) {
      if (amount < 0) throw new InvalidMoneyAmountError("Số tiền không thể âm");
      if (!Number.isFinite(amount)) throw new InvalidMoneyAmountError("Số tiền phải là số hữu hạn");
    }

    public static create(amount: number, currency: Currency): Money {
      return new Money(Math.round(amount * 100) / 100, currency);
    }

    public add(other: Money): Money {
      if (this.currency !== other.currency) throw new CurrencyMismatchError();
      return Money.create(this.amount + other.amount, this.currency);
    }

    public equals(other: Money): boolean {
      return this.amount === other.amount && this.currency === other.currency;
    }
  }
  ```

---

## 3. FINITE STATE MACHINE (FSM) TRONG DOMAIN MODEL

Mọi thực thể có vòng đời thay đổi (Đơn hàng, Lịch hẹn khám, Video playback, Task pipeline, Hóa đơn) bắt buộc phải được mô hình hóa bằng **Finite State Machine**.

### 3.1. MA TRẬN CHUYỂN TRẠNG THÁI (STATE TRANSITION MATRIX)
Trước khi code, phải định nghĩa rõ ma trận chuyển trạng thái:

| Trạng thái hiện tại | Sự kiện kích hoạt (Event) | Điều kiện bảo vệ (Guard) | Trạng thái tiếp theo | Tác vụ đi kèm (Action/Event) |
| :--- | :--- | :--- | :--- | :--- |
| `DRAFT` | `SUBMIT` | Đầy đủ thông tin bắt buộc | `PENDING_REVIEW` | Gửi thông báo quản trị |
| `PENDING_REVIEW` | `APPROVE` | Quyền duyệt hợp lệ | `APPROVED` | Tạo tài khoản, sinh hợp đồng |
| `PENDING_REVIEW` | `REJECT` | Có lý do từ chối | `REJECTED` | Gửi email lý do từ chối |
| `APPROVED` | `ACTIVATE` | Ngày kích hoạt $\le$ hôm nay | `ACTIVE` | Mở quyền truy cập |
| `ACTIVE` | `SUSPEND` | Vi phạm điều khoản | `SUSPENDED` | Khóa phiên đăng nhập |
| `SUSPENDED` | `REINSTATE` | Đã giải quyết vi phạm | `ACTIVE` | Mở lại quyền truy cập |
| `ANY (trừ TERMINAL)` | `TERMINATE` | Yêu cầu hủy vĩnh viễn | `TERMINATED` | Thu hồi tài nguyên, lưu trữ audit |

### 3.2. CÁC QUY TẮC BẮT BUỘC CỦA FSM
1. **Cấm trạng thái lơ lửng (No Floating States)**: Mọi trạng thái của hệ thống phải thuộc tập hợp enum hữu hạn đã định nghĩa. Tuyệt đối không dùng string tự do (`string`).
2. **Strict Transition Blocking (Chặn nghiêm ngặt chuyển trạng thái sai)**: Mọi hành động chuyển trạng thái không nằm trong ma trận chuyển đổi phải bị từ chối ngay lập tức và ném lỗi domain rõ ràng (Ví dụ: Không thể chuyển từ `REJECTED` sang `ACTIVE`).
3. **Guards trước Actions**: Phải kiểm tra toàn bộ điều kiện tiên quyết trước khi thay đổi trạng thái hoặc sinh side-effects.
4. **Terminal State**: Khi thực thể đã vào trạng thái kết thúc (Terminal State như `TERMINATED`, `CANCELLED`, `COMPLETED`), không được phép chấp nhận bất kỳ sự kiện thay đổi trạng thái nào khác.

---

## 4. QUY TẮC BẢO VỆ TÍNH TOÀN VẸN (DEFENSIVE GUARDRAILS)

### 4.1. IDEMPOTENCY (TÍNH BẤT BIẾN KHI THỰC HIỆN LẶP LẠI)
- Mọi thao tác làm thay đổi dữ liệu (tạo mới, thanh toán, duyệt đơn, trừ điểm) phải có cơ chế bảo vệ chống trùng lặp (Idempotency Key hoặc Client Request ID).
- Nếu client gửi lại cùng một yêu cầu (do mạng chập chờn hoặc người dùng bấm 2 lần liên tiếp), hệ thống phải trả về kết quả của lần xử lý đầu tiên mà không thực hiện lại side-effect.

### 4.2. CONCURRENCY & OPTIMISTIC LOCKING
- Với các thực thể bị chỉnh sửa đồng thời bởi nhiều người hoặc tiến trình:
  * Sử dụng trường `version` (hoặc `updated_at`) để thực hiện **Optimistic Locking**.
  * Khi cập nhật: `WHERE id = :id AND version = :expectedVersion`.
  * Nếu số dòng cập nhật $= 0$: Ném ra lỗi xung đột đồng thời `ConcurrencyConflictError` và yêu cầu client nạp lại dữ liệu mới nhất.

### 4.3. TOÁN HỌC VÀ TIỀN TỆ
- **Không bao giờ dùng Float cho tiền tệ**: Luôn lưu trữ tiền tệ dưới dạng số nguyên (cents/xu/đồng) hoặc kiểu Decimal/BigNumber chính xác cao để loại bỏ hoàn toàn sai số dấu phẩy động (`0.1 + 0.2 !== 0.3`).
- **Làm tròn số nhất quán**: Mọi quy tắc làm tròn (Round Half Up, Round Down, Floor) phải được quy định tập trung trong một helper hoặc Value Object duy nhất.

---

## 5. PHÂN LOẠI LỖI NGHIỆP VỤ (DOMAIN ERROR TAXONOMY)

Tuyệt đối không ném lỗi chung chung `new Error("something went wrong")`. Lỗi phải được phân tầng rõ ràng:

1. **DomainError**:
   - Vi phạm quy tắc hoặc invariant của nghiệp vụ.
   - *Ví dụ*: `InsufficientBalanceError`, `OrderAlreadyShippedError`, `AgeRestrictionError`.
   - *HTTP Mapping tương ứng*: `400 Bad Request` hoặc `422 Unprocessable Entity`.
2. **ApplicationError**:
   - Vi phạm luồng sử dụng hoặc không tìm thấy tài nguyên.
   - *Ví dụ*: `EntityNotFoundError`, `UnauthorizedActionError`, `DuplicateKeyError`.
   - *HTTP Mapping tương ứng*: `404 Not Found`, `403 Forbidden`, `409 Conflict`.
3. **InfrastructureError**:
   - Lỗi kết nối kỹ thuật, mạng, cơ sở dữ liệu sập, dịch vụ bên ngoài timeout.
   - *Ví dụ*: `DatabaseConnectionTimeout`, `PaymentGatewayDown`.
   - *HTTP Mapping tương ứng*: `502 Bad Gateway`, `503 Service Unavailable`, `500 Internal Error`.

---

## 6. BUSINESS LOGIC AUDIT CHECKLIST

Trước khi nghiệm thu bất kỳ chức năng nào:
- [ ] Mọi quy tắc nghiệp vụ đã được triển khai trong Domain Entity hoặc Use Case, không nằm trong UI hay Controller.
- [ ] Vòng đời của thực thể đã được mô hình hóa bằng Finite State Machine rõ ràng với danh sách transitions và guards cụ thể.
- [ ] Mọi transition không hợp lệ đều được chứng minh bị chặn lại bằng test tự động.
- [ ] Đã có cơ chế bảo vệ chống trùng lặp yêu cầu (Idempotency) cho các thao tác thay đổi dữ liệu nhạy cảm.
- [ ] Không có tính toán dấu phẩy động (float) cho tiền tệ hoặc số lượng cần độ chính xác tuyệt đối.
- [ ] Toàn bộ lỗi phát sinh đều có mã lỗi nghiệp vụ tường minh (Domain Error Code).
---

## 7. PHỤ LỤC EMBEDDED C++: MÔ HÌNH HÓA ÂM VỊ TIẾNG VIỆT & FSM 4 NHỊP (GACH ENGINE)

### 7.1. VALUE OBJECT ÂM VỊ TIẾNG VIỆT (`PhonicsSyllable`)
- **Đặc tả nghiệp vụ**: Bé 3–6 tuổi đã nhận diện bảng chữ cái; mục tiêu sư phạm là **nhận diện âm vị và ghép vần (Blending)**.
- **Cấu trúc Value Object**:
  * Gồm 3 thành phần chính: **Âm đầu (Onset)**, **Vần (Rhyme)** và **Thanh điệu (Tone)**.
  * Bất biến (Immutable), kích thước nhỏ gọn $le 64	ext{ bytes}$ lưu trữ hoàn toàn trên Stack.
  * Phương thức hành vi nghiệp vụ tự bảo vệ invariants:
    - `is_match(const std::string_view candidate_rhyme) const`: Kiểm tra tính trùng khớp khi bé ghép vần.
    - `full_word() const`: Trả về từ tiếng Việt hoàn chỉnh (ví dụ: `"m"` + `"ít"` $ightarrow$ `"mít"`).

### 7.2. TRIẾT LÝ SƯ PHẠM ANTI-DOPAMINE (RICH DOMAIN RULES)
- **Tuyệt đối không cơ chế kích thích Dopamine ngắn hạn**: Không tính điểm, không đếm sao vàng, không bảng xếp hạng, không âm thanh kích động.
- **Invariant bảo vệ tâm lý trẻ**:
  * Khi bé chọn sai mảnh ghép: Giữ nguyên trạng thái bình yên, không phát âm thanh cảnh báo lỗi (buzzer), không hiện dấu X đỏ, không giới hạn số lần thử.
  * Ngôn từ 100% trong trẻo, gần gũi với đời sống trẻ thơ, tuyệt đối không dùng thuật ngữ giáo điều hay triết học hàn lâm trong app.

### 7.3. MÁY TRẠNG THÁI DÒNG CHẢY DUY NHẤT (UNIFIED 4-BEAT FLOW FSM)
Vòng đời của từng trang truyện được điều khiển bằng FSM hữu hạn với 4 trạng thái bất biến:

| Trạng thái hiện tại | Sự kiện kích hoạt (Event) | Điều kiện bảo vệ (Guard) | Trạng thái tiếp theo | Hành vi nghiệp vụ đi kèm |
| :--- | :--- | :--- | :--- | :--- |
| **OBSERVE** (Nhịp 1) | `OBSERVE_TIMEOUT` | Đã hết thời gian quan sát tĩnh 2.5s | **AWAIT_INPUT** | Mở khóa cảm ứng, vẽ các mảnh vần lựa chọn lên màn hình. |
| **OBSERVE** (Nhịp 1) | `TOUCH_INPUT` | Luôn bị chặn (`is_touch_locked() == true`) | **OBSERVE** | Bỏ qua sự kiện chạm; ngăn bé bấm loạn khi chưa ngắm tranh và nghe lời dẫn. |
| **AWAIT_INPUT** (Nhịp 2) | `SELECT_CHOICE` | Lựa chọn SAI mảnh vần | **AWAIT_INPUT** | Giữ nguyên trạng thái; nhẹ nhàng hướng sự chú ý về các lựa chọn còn lại. |
| **AWAIT_INPUT** (Nhịp 2) | `SELECT_CHOICE` | Lựa chọn ĐÚNG mảnh vần | **TRANSFORM** | Hoàn thành từ vựng; kích hoạt chuyển cảnh sang tranh quả ngọt tử tế. |
| **AWAIT_INPUT** (Nhịp 2) | `HINT_TIMEOUT` | Không có tương tác sau 6.0s | **AWAIT_INPUT** | Nhẹ nhàng làm nổi bật viền của mảnh ghép đúng để dẫn lối cho bé. |
| **TRANSFORM** (Nhịp 3) | `TRANSFORM_FINISHED` | Khoảng lặng chuyển hóa kết thúc | **COMPLETED** | Hiển thị nút mũi tên cho phép bé chủ động lật sang trang tiếp theo. |
| **COMPLETED** (Nhịp 4) | `NEXT_PAGE_TRIGGERED` | Chạm đúng hitbox nút chuyển trang | **OBSERVE** (Trang mới) | Nạp dữ liệu trang tiếp theo từ SQLite và bắt đầu lại từ Nhịp 1. |
| **BẤT KỲ TRẠNG THÁI NÀO** | Chuyển trạng thái bất hợp lệ | Không thỏa mãn Transition Matrix | Giữ nguyên trạng thái | Ghi nhận lỗi domain, cấm nhảy cóc trạng thái. |
