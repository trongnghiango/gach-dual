# TESTING STANDARD
## Universal Testing Pyramid, Quality Gates & Root-Cause Debugging Protocol

---

## 1. TRIẾT LÝ KIỂM THỬ & QUY TẮC BẤT BIẾN (ZERO-TOLERANCE RULES)

1. **Kiểm thử là Bằng chứng Tối thượng**:
   - Một chức năng chưa có test chứng minh thì **coi như chưa hoàn thành**.
   - Không được tuyên bố "hệ thống đã hoạt động" chỉ vì bản thân tin rằng mình đã viết code đúng. Bằng chứng kiểm thử tự động thực tế là điều kiện tiên quyết duy nhất.

2. **Kỷ luật Zero-Hack trong Testing**:
   - **Tuyệt đối cấm**:
     * Sửa đổi câu lệnh assert (kỳ vọng) của test để test "xanh" mà không hiểu tại sao nó đỏ.
     * Tạo mock giả tạo hoặc bypass logic kiểm tra quyền, validation để né lỗi.
     * Sử dụng `.skip()`, `xit()`, hoặc comment out các test đang bị lỗi.
     * Viết test không có assertion thực sự (chỉ chạy qua hàm mà không assert kết quả trả về hoặc side-effects).
   - **Chấp thuận**: Test chỉ có giá trị khi nó có khả năng **FAIL** nếu logic nghiệp vụ bị sai lệch.

---

## 2. KIM TỰ THÁP KIỂM THỬ (THE TESTING PYRAMID)

```text
                  ▲
                 / \
                /   \
               / E2E \       10% Critical User Journeys
              /-------\      (API End-to-End, Full Workflow)
             /         \
            / INTEGRATION\   20% Subsystem Integration
           /--------------\  (Use Cases + Repositories + DB + Adapters)
          /                \
         /   UNIT TESTS     \ 70% Core Domain & Business Logic
        /────────────────────\ (Pure Functions, State Machines, Invariants)
```

---

## 3. CHI TIẾT TỪNG TẦNG KIỂM THỬ

### 3.1. UNIT TESTS (TẦNG NỀN TẢNG — 70%)
- **Mục tiêu**: Đảm bảo toàn bộ logic tính toán và quy tắc nghiệp vụ trong Domain Layer và Pure Functions chạy hoàn toàn chính xác.
- **Phạm vi bắt buộc phủ Unit Test**:
  1. Tất cả các phương thức của **Domain Entities** và **Value Objects**.
  2. Toàn bộ các bước chuyển trạng thái của **Finite State Machine** (Bao gồm cả các trường hợp chuyển trạng thái không hợp lệ phải bị chặn).
  3. Các thuật toán xử lý dữ liệu, tính toán tiền tệ, phân tích cú pháp (Parsers), bộ định dạng (Serializers/Formatters).
  4. Các hàm tiện ích logic thuần (Pure utility helpers).
- **Đặc điểm kỹ thuật**:
  - Tốc độ cực nhanh ($< 5\text{ms}$ mỗi test case).
  - Hoàn toàn độc lập với I/O (Không kết nối Database, không gửi HTTP request, không đọc file system).
  - Không cần framework giả lập phức tạp; ưu tiên test runner tích hợp sẵn (`node:test`, `vitest`, `cargo test`, `go test`, `pytest`).

### 3.2. INTEGRATION TESTS (TẦNG KHUNG XƯƠNG — 20%)
- **Mục tiêu**: Kiểm tra sự phối hợp chính xác giữa Use Cases ở tầng Application với các Adapters ở tầng ngoài.
- **Phạm vi bắt buộc**:
  1. **Use Case Integration**: Kích hoạt Use Case với mock/in-memory adapter để kiểm tra luồng điều phối (Orchestration flow) và kích hoạt Domain Events.
  2. **Repository & Database Integration**: Kiểm thử các câu truy vấn phức tạp, Transaction, Unique Constraint, Foreign Key cascade trên môi trường kiểm thử (In-memory SQLite, Testcontainers PostgreSQL).
  3. **Contract & Schema Validation**: Đảm bảo Schema của DTO chặn đứng dữ liệu rác, sai kiểu hoặc payload chứa trường độc hại.
- **Đặc điểm kỹ thuật**:
  - Đảm bảo cơ chế dọn dẹp (Rollback hoặc Truncate) sau mỗi test case để tránh ô nhiễm dữ liệu giữa các bài kiểm thử.

### 3.3. END-TO-END & WORKFLOW TESTS (TẦNG ĐỈNH CHÓP — 10%)
- **Mục tiêu**: Kiểm chứng toàn bộ chu trình trải nghiệm của người dùng thực tế trên các luồng nghiệp vụ quan trọng nhất (Critical User Journeys).
- **Phạm vi**:
  - Luồng xác thực & cấp quyền (Login -> Refresh Token -> Authorized Action).
  - Luồng giao dịch cốt lõi (Tạo yêu cầu -> Thanh toán/Xử lý -> Hoàn tất -> Kiểm tra dữ liệu lưu vết).
  - Luồng phục hồi lỗi (Mạng rớt -> Báo lỗi thân thiện -> Retry thành công).
- **Đặc điểm kỹ thuật**:
  - Tập trung vào kịch bản (Scenario-driven), hạn chế phụ thuộc vào chi tiết giao diện dễ thay đổi (dùng data-testid hoặc vai trò semantic thay vì CSS selectors mong manh).

---

## 4. QUY TRÌNH FIX BUG 5 BƯỚC (ROOT-CAUSE-FIRST PROTOCOL)

Khi phát hiện bug hoặc nhận báo cáo lỗi, AI Agent bắt buộc phải thực hiện theo quy trình 5 bước:

```text
┌─────────────────┐
│  1. REPRODUCE   │ Viết một Test Case cô lập mô tả lỗi -> Test phải FAIL (RED)
└────────┬────────┘
         ▼
┌─────────────────┐
│ 2. TRACE ROOT   │ Dùng "5 Whys" truy vết nguồn gốc (Domain/UseCase), không dừng ở triệu chứng
└────────┬────────┘
         ▼
┌─────────────────┐
│ 3. FIX AT CORE  │ Sửa lỗi tại đúng tầng kiến trúc căn nguyên; tuyệt đối không bùa code
└────────┬────────┘
         ▼
┌─────────────────┐
│ 4. VERIFY TEST  │ Chạy lại Test Case ở bước 1 -> Test chuyển sang PASS (GREEN)
└────────┬────────┘
         ▼
┌─────────────────┐
│ 5. REGRESSION   │ Chạy toàn bộ Test Suite -> Đảm bảo 100% tests khác vẫn PASS
└─────────────────┘
```

---

## 5. QUY ƯỚC VIẾT TEST & CẤU TRÚC CHUẨN (AAA PATTERN)

Mọi bài kiểm thử phải tuân theo cấu trúc **Arrange — Act — Assert**:

```typescript
describe('Order.complete()', () => {
  it('should transition to COMPLETED and emit OrderCompletedEvent when order is paid and in PROCESSING state', () => {
    // 1. Arrange (Chuẩn bị dữ liệu và trạng thái ban đầu)
    const order = Order.createTestInstance({
      status: OrderStatus.PROCESSING,
      isPaid: true,
    });

    // 2. Act (Thực hiện hành vi nghiệp vụ cần kiểm thử)
    const result = order.complete();

    // 3. Assert (Xác minh kết quả và tác động nghiệp vụ)
    expect(result.isSuccess).toBe(true);
    expect(order.status).toBe(OrderStatus.COMPLETED);
    expect(order.domainEvents).toContainEqual(
      expect.objectContaining({ type: 'ORDER_COMPLETED' })
    );
  });

  it('should reject completion and return DomainError when order is unpaid', () => {
    // 1. Arrange
    const unpaidOrder = Order.createTestInstance({
      status: OrderStatus.PROCESSING,
      isPaid: false,
    });

    // 2. Act
    const result = unpaidOrder.complete();

    // 3. Assert
    expect(result.isFailure).toBe(true);
    expect(result.error.code).toBe('ORDER_UNPAID');
    expect(unpaidOrder.status).toBe(OrderStatus.PROCESSING); // Trạng thái không bị thay đổi lén lút
  });
});
```

### Quy tắc đặt tên Test:
- Đặt tên theo mẫu: `should_<hành_vi_kỳ_vọng>_when_<điều_kiện_đầu_vào_hoặc_ngữ_cảnh>`
- Tên test phải đọc như một câu hoàn chỉnh thể hiện rõ đặc tả nghiệp vụ.

---

## 6. KIỂM SOÁT TÍNH ĐỘC LẬP & CHỐNG FLAKY TESTS

1. **Deterministic Execution (Tính tất định)**:
   - Test phải cho kết quả giống hệt nhau ở mọi lần chạy trên mọi máy tính.
   - Tuyệt đối không dùng `setTimeout` với thời gian ước lượng để chờ bất đồng bộ. Dùng `waitFor`, `EventEmitter.once`, hoặc Promise resolving có cơ chế timeout xác định.
2. **Clock & Time Freezing**:
   - Mọi logic phụ thuộc vào thời gian thực (`Date.now()`, `new Date()`) phải được inject qua Clock Service/Interface để có thể đóng băng thời gian trong test.
3. **Resource Leak Guard trong Tests**:
   - Luôn thu hồi và dọn dẹp các handle, worker pool, database connection sau mỗi suite (`afterAll`, `afterEach`, `teardown`).

---

## 7. QUALITY GATES & STOP CONDITIONS

Trước khi hoàn tất bất kỳ task nào, hệ thống phải vượt qua toàn bộ các cổng chất lượng sau:
- [ ] **G0: Compilation Gate**: Toàn bộ codebase biên dịch thành công 100% không có lỗi type (`tsc --noEmit`, `cargo check`).
- [ ] **G1: Linter & Style Gate**: Không có lỗi linter nghiêm trọng hoặc cảnh báo biến không sử dụng.
- [ ] **G2: Unit Test Gate**: 100% Unit Tests pass với thời gian thực thi tối ưu.
- [ ] **G3: Integration Gate**: 100% Integration Tests pass, database transactions được rollback sạch sẽ.
- [ ] **G4: Regression Gate**: Toàn bộ test suite của toàn dự án pass 100% (Không gây gãy đổ tính năng cũ).
- [ ] **G5: Zero-Suppression Gate**: Không có test nào bị tắt (`.skip`), không có `@ts-ignore` mới được thêm vào.
---

## 8. PHỤ LỤC EMBEDDED C++: KIM TỰ THÁP KIỂM THỬ TRÊN THIẾT BỊ NHÚNG (GACH ENGINE)

### 8.1. PHÂN BỔ KIM TỰ THÁP KIỂM THỬ TRONG C++ NATIVE
1. **70% Unit Tests (Host-Side Pure C++17)**:
   - Thư mục: `tests/unit/`.
   - Đối tượng: Toàn bộ phương thức của `PhonicsSyllable`, ma trận chuyển trạng thái của `FlowFSM`, tính toán tọa độ hitbox của `UILayout`.
   - Yêu cầu: Không phụ thuộc thư viện kiểm thử nặng nề; sử dụng test runner siêu tốc tích hợp (`assert` hoặc test macro tối giản).
   - Tốc độ: Toàn bộ suite unit tests phải thực thi xong trong $< 15	ext{ ms}$ trên máy host.
2. **20% Integration Tests (Repository & Use Case Orchestration)**:
   - Thư mục: `tests/integration/`.
   - Đối tượng: `SqliteStoryRepository` kết nối với SQLite in-memory (`:memory:`), kiểm tra toàn vẹn ràng buộc 3NF và foreign keys; `StoryInteractor` phối hợp giữa FSM và Repository.
   - Yêu cầu: Đảm bảo tự động dọn dẹp bộ nhớ và đóng kết nối cơ sở dữ liệu sau mỗi test case.
3. **10% E2E Workflow Tests (Full Story Loop)**:
   - Thư mục: `tests/e2e/`.
   - Đối tượng: Giả lập trọn vẹn một hành trình đọc truyện của bé từ Trang 1 đến Trang cuối cùng, bao gồm cả các tình huống chạm sai, đợi timeout tự gợi ý, hoàn thành từ và chuyển trang.

### 8.2. QUALITY GATES BẮT BUỘC CHO HỆ THỐNG NHÚNG KINDLE
- **Gate M0 (AddressSanitizer / UndefinedBehaviorSanitizer)**:
  * Lệnh kiểm tra: `g++ -fsanitize=address,undefined ...`
  * Yêu cầu: 0 memory leaks, 0 heap-use-after-free, 0 out-of-bounds, 0 undefined behavior.
- **Gate M1 (RAM Ceiling Audit - RSS < 12MB)**:
  * Kiểm tra qua công cụ đo bộ nhớ (Massif / `getrusage()`): Mức đỉnh RAM (Peak RSS) của toàn ứng dụng tuyệt đối không vượt quá $12	ext{ MB}$.
- **Gate M2 (Cross-Compilation Check)**:
  * Xác nhận mã nguồn biên dịch sạch sẽ không warning khi target sang kiến trúc ARMv7 hard-float (Cortex-A9 i.MX6).
