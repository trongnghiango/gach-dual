# AUTONOMOUS ENGINEERING LOOP
## Universal Autonomous Protocol for System Evolution, Architecture Integrity & Zero-Hack Engineering

---

## 0. MỆNH LỆNH TỐI CAO & QUY CHUẨN TỰ CHỦ

1. **Vận hành Tự chủ Chủ động (Proactive Autonomy)**:
   - Từ thời điểm này, AI Agent không được thụ động chờ người dùng chỉ dẫn từng bước nhỏ hoặc giải quyết từng file rời rạc.
   - **Trách nhiệm tối cao**: Tự chủ đưa hệ thống từ trạng thái hiện tại đến trạng thái hoàn thiện cao nhất có thể đạt được trong phạm vi kiến trúc, nghiệp vụ và mục tiêu của dự án.
   - Không dừng lại chỉ vì "đã gõ xong code" hoặc "mã nguồn biên dịch không báo lỗi". Chỉ dừng lại khi thỏa mãn toàn bộ **Điều kiện Dừng (Stop Condition)** và **Definition of Done (DoD)**.

2. **Khai thác Tối đa Công cụ Môi trường (Harness & Tooling Discipline)**:
   - **`todo_write`**: Bắt buộc đồng bộ danh sách công việc và trạng thái (`pending`, `in_progress`, `completed`) xuyên suốt quá trình thực thi. Luôn cập nhật tiến độ trước và sau mỗi bước can thiệp.
   - **`read` & `edit`**: Luôn dùng `read` để quan sát toàn bộ ngữ cảnh xung quanh trước khi sửa. Dùng `edit` để thay thế chuẩn xác từng khối mã mục tiêu. Tuyệt đối không ghi đè bừa bãi.
   - **`glob` & `grep`**: Sử dụng công cụ tìm kiếm tệp và nội dung chuyên dụng thay vì dùng shell commands thô sơ.
   - **`ask_user_question`**: Chỉ tương tác xin quyết định khi gặp rẽ nhánh nghiệp vụ cốt lõi không thể tự suy luận, hoặc cần phê duyệt các hành động có rủi ro cao. Gom câu hỏi thành trắc nghiệm súc tích, có giải thích trade-off rõ ràng.

---

## 1. TỔNG QUAN CHU TRÌNH 7 PHA (THE 7-PHASE ENGINE)

```text
┌────────────────────────────────────────────────────────────────────────┐
│  PHASE 0: INTAKE & SCOPING GATE (Thẩm định Đầu vào & Xác lập Phạm vi)  │
│  - Phân tích prompt: Actor, Trigger, Inputs, Expected Outputs, Bounds  │
│  - Phân loại Tier (Tier 1: Quick-Fix | Tier 2: Feature | Tier 3: Core)  │
└───────────────────────────────────┬────────────────────────────────────┘
                                    │
┌───────────────────────────────────▼────────────────────────────────────┐
│  PHASE 1: CODEBASE DISCOVERY & ARCHITECTURE CALIBRATION (Rà soát Repo) │
│  - Quét tệp cấu hình, dependencies, test runner và schema hiện hữu     │
│  - Ánh xạ mã nguồn vào 4 tầng Clean Architecture                       │
│  - Xác định Single Source of Truth của hệ thống                        │
└───────────────────────────────────┬────────────────────────────────────┘
                                    │
┌───────────────────────────────────▼────────────────────────────────────┐
│  PHASE 2: SYSTEM GAP MAP & TASK SYNCHRONIZATION (Lộ trình & Đồng bộ)  │
│  - Khảo sát khoảng trống 6 chiều (Arch, Biz, UI/UX, Test, Perf, Docs)  │
│  - Xếp hạng ưu tiên theo mức độ P0 -> P3                               │
│  - Đồng bộ danh sách công việc vào todo_write                          │
└───────────────────────────────────┬────────────────────────────────────┘
                                    │
┌───────────────────────────────────▼────────────────────────────────────┐
│  PHASE 3: CLEAN ARCHITECTURE DESIGN & CONTRACTS (Thiết kế & Hợp đồng)  │
│  - Mô hình hóa Pure Domain Model & State Machine (FSM)                 │
│  - Thiết lập Inbound/Outbound Ports, DTOs & Validation Schemas         │
│  - Phân lập rõ ràng ranh giới giữa Domain, App, Adapters và Infra     │
└───────────────────────────────────┬────────────────────────────────────┘
                                    │
┌───────────────────────────────────▼────────────────────────────────────┐
│  PHASE 4: ZERO-HACK IMPLEMENTATION & TDD (Triển khai & Kiểm thử)       │
│  - Áp dụng TDD: Viết test chứng minh thất bại trước (Red Phase)        │
│  - Triển khai logic sạch, loại bỏ triệt để triệu chứng (Green Phase)   │
│  - Tối ưu hóa cấu trúc mà không làm vỡ kiến trúc (Refactor Phase)      │
└───────────────────────────────────┬────────────────────────────────────┘
                                    │
┌───────────────────────────────────▼────────────────────────────────────┐
│  PHASE 5: MULTI-PERSPECTIVE VERIFICATION (Thẩm định Đa chiều)          │
│  - Chạy toàn diện Unit, Integration, UI/UX và Regression Tests         │
│  - Kiểm tra rò rỉ tài nguyên: Event listeners, Timers, Memory, DB Conns│
│  - 5-Persona Self-Audit: Architect, QA, Security, Perf, Maintainer     │
└───────────────────────────────────┬────────────────────────────────────┘
                                    │
┌───────────────────────────────────▼────────────────────────────────────┐
│  PHASE 6: STOP CONDITION & FINAL HANDOFF (Điều kiện Dừng & Nghiệm thu) │
│  - Xác thực Definition of Done (DoD) toàn diện                         │
│  - Cập nhật tài liệu kỹ thuật đồng bộ với mã nguồn                     │
│  - Xuất báo cáo kỹ thuật hoàn chỉnh (Root Cause, Changes, Verification)│
└────────────────────────────────────────────────────────────────────────┘
```

---

## 2. CHI TIẾT TỪNG PHA THỰC THI

### PHASE 0: INTAKE & SCOPING GATE
1. **Kiểm tra tính đầy đủ của yêu cầu**:
   - **Actor**: Ai hoặc hệ thống nào kích hoạt luồng này?
   - **Trigger**: Sự kiện kích hoạt là gì (API request, user click, cron job, domain event)?
   - **Input Data**: Dữ liệu đầu vào gồm những gì? Schema xác thực ra sao?
   - **Output Expectation**: Dữ liệu trả về hoặc trạng thái đích cần đạt?
   - **Constraints & Edge cases**: Giới hạn thời gian, tải, đồng thời, trạng thái rỗng, lỗi mạng.
2. **Phân loại Tier thực thi**:
   - **Tier 1 (Targeted / Quick Fix)**: Vá lỗi cục bộ, sửa cú pháp, chỉnh cấu hình nhỏ không ảnh hưởng hợp đồng API.
   - **Tier 2 (Feature / Module Implementation)**: Thêm mới hoặc mở rộng Use Case, Entity, Endpoint, Component UI.
   - **Tier 3 (Core Architectural Overhaul)**: Tái cấu trúc phân tầng, đổi cơ chế quản lý trạng thái, refactor database schema cốt lõi.

---

### PHASE 1: CODEBASE DISCOVERY & ARCHITECTURE CALIBRATION
1. **Khảo sát cấu trúc tệp (Tooling: `glob`, `read`)**:
   - Quét tìm tệp gốc quản lý dự án: `package.json`, `Cargo.toml`, `go.mod`, `pom.xml`, `pyproject.toml`, `tsconfig.json`.
   - Xác định: Ngôn ngữ, Framework, Package Manager, ORM/DB, Testing Framework, Linter.
2. **Lập bản đồ 4 tầng Clean Architecture**:
   - **Domain Layer**: Entities, Value Objects, Enums, Domain Rules, State Machine.
   - **Application Layer**: Use Cases, Interactors, Command/Query Handlers, Inbound/Outbound Ports.
   - **Adapters Layer**: Repositories, Controllers, Presenters, Serializers, Gateways.
   - **Infrastructure Layer**: Framework routing, Database driver, File system, Network, OS Bridge.
3. **Xác định Single Source of Truth**:
   - Nhận diện cơ chế lưu trữ dữ liệu thật sự và tránh tình trạng trạng thái phân mảnh (Split-brain state).

---

### PHASE 2: SYSTEM GAP MAP & TASK SYNCHRONIZATION
1. **Lập bản đồ khoảng trống (Gap Discovery Matrix)**:
   - *Architecture Gap*: Có rò rỉ abstraction, vi phạm Dependency Rule hay import vòng lặp không?
   - *Business Logic Gap*: Các trạng thái lỗi, timeout, xung đột đồng thời, idempotency đã được xử lý chưa?
   - *UI/UX Gap*: Đã có đủ 4 trạng thái: Empty, Loading, Error, Success chưa?
   - *Testing Gap*: Tầng nghiệp vụ cốt lõi đã có test chứng minh chưa?
   - *Reliability & Resource Gap*: Có bộ nhớ, kết nối socket, file descriptor hay event listener nào chưa giải phóng?
2. **Xếp hạng ưu tiên (Priority Queue)**:
   - `P0 - Blocker`: Lỗi crash, hỏng luồng thanh toán/dữ liệu chính, vi phạm bảo mật, vỡ kiến trúc nền tảng.
   - `P1 - Critical Workflow`: Thiếu xử lý lỗi quan trọng, thiếu validation, thiếu integration test chính.
   - `P2 - Code Health`: Refactor mã lặp, cải thiện hiệu năng, chuẩn hóa naming convention.
   - `P3 - Polish`: Cải tiến giao diện phụ, tinh chỉnh log, tối ưu tài liệu.
3. **Đồng bộ hóa vào `todo_write`**:
   - Phân rã mục tiêu thành các đầu việc nguyên tử (atomic steps).
   - Duy trì đúng 1 việc `in_progress` tại một thời điểm (trừ khi chạy song song qua subagents).

---

### PHASE 3: CLEAN ARCHITECTURE DESIGN & CONTRACTS
1. **Thiết kế Domain Purity**:
   - Domain Model là trung tâm tối thượng: Tuyệt đối không import thư viện bên ngoài, ORM annotations, HTTP headers hay UI components vào Domain.
   - Mô hình hóa vòng đời bằng **Finite State Machine (FSM)** tường minh. Mọi bước chuyển trạng thái không hợp lệ phải bị chặn đứng (fail-fast).
2. **Thiết kế Contract-First**:
   - Định nghĩa trước Schema xác thực dữ liệu vào/ra (Zod, TypeBox, JSON Schema, Protobuf).
   - Thiết lập Interfaces (Ports) cho Repositories và Third-party Adapters trước khi viết code triển khai.

---

### PHASE 4: ZERO-HACK IMPLEMENTATION & TDD
1. **Kỷ luật TDD (Test-Driven Development)**:
   - **Red**: Viết unit test cho Use Case/Domain dựa trên hợp đồng đã thiết kế -> Chạy test và chứng minh test FAIL đúng lý do.
   - **Green**: Viết code triển khai sạch sẽ, ngắn gọn, tuân thủ SOLID -> Chạy test và chứng minh test PASS.
   - **Refactor**: Tối ưu mã nguồn, tách nhỏ hàm, loại bỏ duplicate code -> Đảm bảo test vẫn PASS 100%.
2. **Quy tắc Zero-Hack (Tuyệt đối cấm bùa code)**:
   - Cấm dùng `// @ts-ignore`, cấm ép kiểu `as any` vô căn cứ.
   - Cấm thêm `if/else` để che giấu giá trị `null/undefined` bất thường mà không tìm hiểu tại sao giá trị đó xuất hiện.
   - Cấm sửa assertion của test hoặc tạo mock dữ liệu dối trá chỉ để pass test runner.

---

### PHASE 5: MULTI-PERSPECTIVE VERIFICATION
1. **Kiểm thử tự động đa tầng**:
   - Chạy toàn bộ Unit Tests và Integration Tests.
   - Chạy Typecheck (`tsc --noEmit`, `cargo check`, v.v.) và Linter.
   - Kiểm tra Regression: Đảm bảo các module kế cận không bị ảnh hưởng phụ (Side-effects).
2. **5-Persona Self-Audit (Tự phản biện 5 góc nhìn)**:
   - *Principal Architect*: Mã nguồn có tuân thủ The Dependency Rule không? Ranh giới các tầng có bị rò rỉ không?
   - *QA Engineer*: Đã thử dữ liệu biên, chuỗi rỗng, timeout, mạng rớt, thao tác người dùng bấm liên tục (rapid clicks) chưa?
   - *Security Engineer*: Có rủi ro injection, path traversal, lộ secret key, thiếu sanitize input không?
   - *Performance Engineer*: Có N+1 queries, unindexed search, unthrottled loop, leak bộ nhớ không?
   - *Maintainer*: Mã nguồn có trong sáng, dễ hiểu, tuân thủ Ubiquitous Language không?

---

### PHASE 6: STOP CONDITION & FINAL HANDOFF
1. **Checklist Điều kiện Dừng (Stop Condition Checklist)**:
   - [ ] Không còn lỗi P0 hoặc P1 nào chưa được giải quyết trong phạm vi task.
   - [ ] 100% tests liên quan vượt qua mà không có test nào bị vô hiệu hóa hoặc fake data.
   - [ ] Build & Typecheck pass 100% với 0 warning/error nghiêm trọng.
   - [ ] Toàn bộ tài nguyên cấp phát (Object URLs, Audio/Video contexts, Event Listeners, Sockets, DB pools) được dọn dẹp sạch sẽ khi hủy.
   - [ ] Toàn bộ mục trong `todo_write` đã chuyển sang `completed`.
   - [ ] Tài liệu kỹ thuật liên quan đã được đồng bộ với mã nguồn thực tế.
2. **Báo cáo Nghiệm thu Cuối cùng (Final Handoff Report)**:
   - Trình bày rõ ràng:
     1. Tóm tắt kết quả (Summary).
     2. Bản chất vấn đề & Phân tích nguyên nhân gốc rễ (Root Cause Analysis).
     3. Các quyết định kiến trúc & Thay đổi mã nguồn (Architecture & Implementation).
     4. Bằng chứng kiểm thử (Verification & Test Evidence).
     5. Rủi ro còn lại & Đề xuất tiếp theo (Remaining Backlog).

---

## 3. BẢO VỆ TÀI NGUYÊN & RESOURCE LIFECYCLE GUARD

Mọi hệ thống có tài nguyên động phải tuân thủ nghiêm ngặt nguyên tắc **Cấp phát đi đôi với Thu hồi (RAII / Lifecycle Guard)**:
1. **Frontend / UI**:
   - Mọi `addEventListener` phải có `removeEventListener` tương ứng trong cleanup function (`useEffect`, `onUnmount`).
   - Mọi `setInterval`, `setTimeout` phải được clear khi hủy component.
   - Mọi URL tạo bởi `URL.createObjectURL(blob)` bắt buộc phải được thu hồi bằng `URL.revokeObjectURL(url)` khi đối tượng bị thay thế hoặc hủy bỏ.
   - Mọi kết nối WebSocket/WebRTC/SSE phải gửi lệnh disconnect/close tường minh.
2. **Backend / System**:
   - Kết nối cơ sở dữ liệu và Transaction phải luôn nằm trong block `try...finally` để bảo đảm đóng kết nối hoặc rollback khi xảy ra exception.
   - File descriptors, Stream handlers, Child processes phải được close/kill an toàn khi hoàn tất.

---

## 4. CƠ CHẾ TỰ SỬA LỖI & PHẢN HỒI (SELF-CORRECTION PROTOCOL)

Khi gặp lỗi trong quá trình thực thi:
1. **Quy tắc 5 Whys (Năm câu hỏi Tại sao)**:
   - Không được sửa ngay tại triệu chứng bề mặt.
   - Hỏi liên tiếp "Tại sao điều này xảy ra?" cho đến khi chạm tới nguồn gốc sâu xa (thiếu validation đầu vào, state machine thiếu guard, race condition trong bất đồng bộ, sai lệch contract).
2. **Bảo vệ chống vòng lặp vô hạn (Infinite Loop Breaker)**:
   - Nếu thử sửa 1 lỗi 3 lần liên tiếp mà test vẫn fail cùng một triệu chứng:
     * DỪNG LẠI ngay lập tức.
     * Đọc lại toàn bộ call stack và dữ liệu thực tế tại runtime.
     * Kiểm tra lại giả định ban đầu về kiến trúc hoặc thư viện.
     * Refactor lại giải pháp theo hướng đơn giản, minh bạch hơn thay vì tiếp tục chắp vá.
---

## 5. PHỤ LỤC EMBEDDED C++: CHU TRÌNH 7 PHA TRÊN THIẾT BỊ NHÚNG KINDLE

Trong môi trường hệ thống nhúng C++17 hạn chế tài nguyên:

1. **PHASE 0 (Intake & Hardware Scoping)**:
   - Thẩm định các giới hạn vật lý: RAM khả dụng cho app $< 12	ext{ MB}$, CPU 1 nhân 1.0 GHz, phân vùng FAT32 (`access(F_OK)`), hoàn toàn không có âm thanh.
2. **PHASE 1 (Toolchain & Environment Calibration)**:
   - Xác lập 2 môi trường biên dịch song song:
     * **Host Toolchain (x86_64)**: Phục vụ chu trình TDD siêu tốc, chạy AddressSanitizer và linter.
     * **Target Toolchain (arm-linux-gnueabihf / Docker)**: Phục vụ đóng gói binary chạy thực tế trên Kindle PW2/PW3.
3. **PHASE 2 (System Gap Map & Resource Budgeting)**:
   - Định lượng ngân sách tài nguyên trước khi code: RAM phân bổ cho ảnh, font glyphs, SQLite cache (khóa 500KB) và khung hình Cairo.
4. **PHASE 3 (Pure Domain Design & Contracts)**:
   - Mô hình hóa Value Objects trên Stack, thiết kế Inbound/Outbound Ports bằng C++ Pure Virtual Interfaces.
5. **PHASE 4 (Zero-Hack TDD on Host)**:
   - Viết test trước tại `tests/unit/` $ightarrow$ Chạy fail (Red) $ightarrow$ Viết code Domain $ightarrow$ Chạy pass (Green) $ightarrow$ Tối ưu hóa không cấp phát heap trong hot-path (Refactor).
6. **PHASE 5 (Multi-Perspective Embedded Verification)**:
   - Kiểm tra rò rỉ bộ nhớ bằng AddressSanitizer (`-fsanitize=address,undefined`).
   - Kiểm tra đỉnh RAM (Peak RSS) $< 12	ext{ MB}$.
   - Kiểm tra thời gian khởi động app $< 1.5	ext{s}$ và thời gian nạp trang truyện $< 30	ext{ ms}$.
7. **PHASE 6 (KUAL Package & Deployment Verification)**:
   - Đóng gói cấu trúc KUAL extension (`extension/gach/`).
   - Kiểm tra script khởi động an toàn trên phân vùng FAT32: cấm dùng `access(X_OK)`, bắt buộc dùng `access(F_OK)`.
