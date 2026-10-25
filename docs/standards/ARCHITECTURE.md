# SYSTEM ARCHITECTURE STANDARD
## Clean Architecture, Modular Boundary & Domain-Driven System Blueprint

---

## 1. NGUYÊN TẮC KIẾN TRÚC CỐT LÕI (CORE ARCHITECTURAL PRINCIPLES)

Mọi hệ thống phần mềm (Web, Backend, Frontend, Desktop, Mobile, CLI) được phát triển hoặc tái cấu trúc bởi AI Agent đều phải tuân thủ nghiêm ngặt các nguyên tắc sau:

1. **Quy tắc Phụ thuộc (The Dependency Rule)**:
   - *Mã nguồn ở tầng bên trong tuyệt đối không được biết và không được phụ thuộc vào mã nguồn ở tầng bên ngoài.*
   - Mã nguồn ở tầng ngoài phụ thuộc vào tầng trong thông qua Interfaces (Ports & Inversion of Control).
2. **Độc lập với Framework & Thư viện (Framework Independence)**:
   - Kiến trúc không lệ thuộc vào bất kỳ framework cụ thể nào (Express, Nest, Next, Fastify, Spring, Actix, v.v.). Framework chỉ là chi tiết triển khai ở tầng Infrastructure.
3. **Độc lập với Giao diện (UI Independence)**:
   - Giao diện người dùng có thể thay đổi (từ Web sang Desktop, CLI hoặc API) mà không làm thay đổi logic nghiệp vụ cốt lõi.
4. **Độc lập với Cơ sở Dữ liệu (Database Independence)**:
   - Nghiệp vụ cốt lõi không bị ràng buộc bởi PostgreSQL, MySQL, MongoDB hay SQLite. Có thể đổi loại database mà không phải sửa Domain logic.
5. **Kiểm thử độc lập (Testability)**:
   - Toàn bộ nghiệp vụ trong Domain và Application Layer có thể được kiểm thử 100% bằng Unit Tests mà không cần khởi động Database, Web server hay dịch vụ bên ngoài.

---

## 2. KIẾN TRÚC PHÂN TẦNG 4 LỚP (THE 4-TIER CLEAN ARCHITECTURE)

```text
┌────────────────────────────────────────────────────────────────────────┐
│                        4. INFRASTRUCTURE LAYER                         │
│  - Web Framework (Fastify, Express, Next.js, Vite, Actix)              │
│  - Database Drivers & ORM Engines (Drizzle, Prisma, TypeORM, SQLx)     │
│  - Third-party APIs, HTTP Clients, File System, OS Bridges             │
└───────────────────────────────────┬────────────────────────────────────┘
                                    │ Configures & Injects
                                    ▼
┌────────────────────────────────────────────────────────────────────────┐
│                        3. ADAPTERS LAYER (INTERFACE)                   │
│  - Controllers, REST / GraphQL / tRPC Handlers                         │
│  - Repositories Implementation (PostgresOrderRepo, S3StorageRepo)      │
│  - Presenters, DTO Mappers, Serialization / Deserialization            │
└───────────────────────────────────┬────────────────────────────────────┘
                                    │ Implements Ports
                                    ▼
┌────────────────────────────────────────────────────────────────────────┐
│                      2. APPLICATION LAYER (USE CASES)                  │
│  - Use Cases / Interactors / Services (CreateOrder, ProcessPayment)    │
│  - Inbound Ports (Use Case Interfaces)                                 │
│  - Outbound Ports (Repository Interfaces, External Service Interfaces) │
│  - DTOs, Command / Query Data Structures                               │
└───────────────────────────────────┬────────────────────────────────────┘
                                    │ Orchestrates
                                    ▼
┌────────────────────────────────────────────────────────────────────────┐
│                          1. DOMAIN LAYER (CORE)                        │
│  - Pure Domain Entities & Value Objects                                │
│  - Pure Business Rules & Invariants                                    │
│  - Finite State Machines (FSM) & Domain Events                         │
│  - Zero External Dependencies (Không ORM, Không Web, Không IO)         │
└────────────────────────────────────────────────────────────────────────┘
```

---

## 3. CHI TIẾT TRÁCH NHIỆM TỪNG TẦNG

### 3.1. DOMAIN LAYER (TRÁI TIM NGHIỆP VỤ)
- **Entities**: Đối tượng mang bản sắc nghiệp vụ (Identity) và vòng đời thay đổi (ví dụ: `User`, `Order`, `Project`).
- **Value Objects**: Đối tượng định nghĩa bằng giá trị của nó, bất biến (Immutable), tự thẩm định tính hợp lệ khi khởi tạo (ví dụ: `Email`, `Money`, `DateRange`).
- **Domain Business Rules**: Các quy tắc nghiệp vụ bất biến không phụ thuộc vào công nghệ (ví dụ: "Đơn hàng chỉ được hủy khi ở trạng thái Chờ xử lý", "Giảm giá tối đa không vượt quá 50%").
- **Finite State Machine**: Mọi biến chuyển trạng thái phức tạp phải được biểu diễn qua State Machine tường minh (Valid Transitions, Guards, Events).
- **Domain Events**: Sự kiện phản ánh điều gì đã xảy ra trong Domain (ví dụ: `OrderPlacedEvent`, `PaymentFailedEvent`).
- **Kỷ luật**: Tuyệt đối không import bất kỳ package nào từ npm/crates ngoại trừ các helper logic thuần (pure utility) hoặc schema definition thuần.

### 3.2. APPLICATION LAYER (ĐIỀU PHỐI USE CASES)
- **Use Cases / Interactors**: Chịu trách nhiệm thực thi một hành vi cụ thể của hệ thống (ví dụ: `RegisterUserUseCase`, `CheckoutCartUseCase`).
- **Orchestration**: Nhận input -> gọi Domain logic -> tương tác với Outbound Ports (Repositories, Notification) -> trả về kết quả.
- **Inbound Ports**: Định nghĩa giao diện đầu vào mà Controller gọi đến.
- **Outbound Ports**: Định nghĩa giao diện đầu ra mà Application yêu cầu tầng ngoài thực hiện (ví dụ: `IOrderRepository`, `IEmailSender`).
- **Transaction Boundary**: Tầng xác lập phạm vi Transaction của cơ sở dữ liệu (Unit of Work).

### 3.3. ADAPTERS LAYER (CẦU NỐI GIAO DIỆN)
- **Inbound Adapters**:
  - Web Controllers, Route Handlers, CLI Commands, Event Subscribers.
  - Nhận HTTP/RPC/CLI request -> thẩm định cú pháp schema (Zod, TypeBox) -> chuyển thành Use Case Command -> gọi Use Case.
- **Outbound Adapters**:
  - Triển khai cụ thể của Outbound Ports (ví dụ: `DrizzleOrderRepository` thực thi `IOrderRepository`).
  - Giao tiếp với Database, Cache (Redis), File storage (S3), Mail gateway.
- **Mappers & Presenters**:
  - Chuyển đổi dữ liệu giữa Domain Entities và DTO/Database Rows. Tuyệt đối không để lộ cấu trúc Entity hoặc DB Schema trực tiếp ra ngoài API.

### 3.4. INFRASTRUCTURE LAYER (HẠ TẦNG VẬN HÀNH)
- Thiết lập kết nối cơ sở dữ liệu, connection pool, migrations.
- Cấu hình server HTTP, WebSocket, Middleware (CORS, Rate Limiting, Authentication tokens).
- Tích hợp hệ điều hành, File System, môi trường Runtime (Node.js, Bun, Rust, Go).

---

## 4. LUỒNG DỮ LIỆU & RANH GIỚI VƯỢT TẦNG (CROSS-BOUNDARY DATA FLOW)

```text
[HTTP Request]
       │
       ▼
[Inbound Controller / Adapter]
       │ 1. Validate Input (DTO Schema)
       ▼
[Inbound Port / Use Case]
       │ 2. Fetch Entity via Outbound Port
       ▼
[Outbound Port (Interface)] ◄──── [Repository Adapter (Impl)] ────► [Database]
       │ 3. Return Entity
       ▼
[Domain Entity / State Machine]
       │ 4. Execute Business Logic & Validate Invariants
       ▼
[Use Case]
       │ 5. Save Entity via Outbound Port
       │ 6. Emit Domain Events
       ▼
[Presenter / DTO Mapper]
       │ 7. Map to Output DTO / JSON
       ▼
[HTTP Response]
```

### Quy tắc vượt ranh giới:
1. **Dữ liệu đi qua ranh giới phải là DTOs hoặc Primitive Types**: Không truyền trực tiếp đối tượng DB (ORM instances) hoặc đối tượng Request/Response của framework vào Use Case hoặc Domain.
2. **Mapping hai chiều**:
   - Controller Map Request -> Use Case Input DTO.
   - Use Case làm việc với Domain Entity.
   - Repository Adapter Map Entity -> Database Row và ngược lại.
   - Presenter Map Domain Result -> Response DTO.

---

## 5. CONTRACT-FIRST DESIGN & QUẢN TRỊ GIAO THỨC

Mọi API giữa Backend và Frontend, hoặc giữa các Service phải được thiết kế theo nguyên tắc **Contract-First**:
1. **Schema Validation**:
   - Sử dụng Schema định kiểu mạnh (Zod, TypeBox, OpenAPI, Protobuf) làm Single Source of Truth cho hợp đồng giao tiếp.
   - Mọi endpoint phải có Schema xác thực chặt chẽ cho: `Params`, `Query`, `Body` và `Response`.
2. **Error Format chuẩn hóa**:
   Mọi phản hồi lỗi đều phải tuân theo cấu trúc thống nhất:
   ```json
   {
     "success": false,
     "error": {
       "code": "DOMAIN_VALIDATION_ERROR",
       "message": "Mô tả lỗi dễ hiểu cho người dùng",
       "details": [
         { "field": "amount", "issue": "Giá trị phải lớn hơn 0" }
       ],
       "timestamp": "2026-10-07T12:00:00.000Z"
     }
   }
   ```

---

## 6. CÁC NGUY HIỂM & ANTI-PATTERNS CẦN TRÁNH (ZERO TOLERANCE)

1. **Anemic Domain Model (Domain rỗng tuếch)**:
   - Entities chỉ là các class chứa getter/setter hoặc interface dữ liệu thụ động, trong khi toàn bộ nghiệp vụ nằm rải rác ở Controller hoặc Helper.
   - *Chuẩn mực*: Chuyển toàn bộ logic nghiệp vụ, tính toán và bảo vệ tính toàn vẹn vào chính Domain Entity.
2. **Fat Controllers (Controller phình to)**:
   - Controller chứa logic tính toán, gọi DB trực tiếp, xử lý transactions.
   - *Chuẩn mực*: Controller chỉ phân tích cú pháp HTTP, gọi Use Case và trả về HTTP status code phù hợp.
3. **Database Leaks (Rò rỉ cơ sở dữ liệu vào Domain)**:
   - Domain Model chứa decorator ORM (`@Entity`, `@Column`), hoặc trực tiếp gọi hàm `save()`.
   - *Chuẩn mực*: Tách biệt hoàn toàn Domain Entity khỏi Database Table Schema.
4. **Circular Dependencies (Phụ thuộc vòng tròn)**:
   - File A import File B, và File B lại import ngược lại File A.
   - *Chuẩn mực*: Dùng Interface/Port để đảo ngược phụ thuộc (Inversion of Control) hoặc tách module chung.

---

## 7. ARCHITECTURAL COMPLIANCE CHECKLIST

Trước khi coi một tính năng là hoàn tất, hãy kiểm tra danh mục sau:
- [ ] Domain Model không chứa bất kỳ import nào từ Framework, Web, hay ORM.
- [ ] Toàn bộ Use Cases phụ thuộc vào Interfaces (Ports), không phụ thuộc vào lớp triển khai cụ thể (Adapters).
- [ ] Không có mã nguồn nào ở tầng trong import mã nguồn ở tầng ngoài.
- [ ] DTOs và Mappers ngăn chặn triệt để việc rò rỉ Entity hoặc Database Model ra bên ngoài API.
- [ ] Mọi luồng xử lý đều có Unit Test kiểm thử logic nghiệp vụ ở tầng Domain và Application mà không cần mock framework HTTP.
---

## 8. PHỤ LỤC EMBEDDED C++: CLEAN ARCHITECTURE TRÊN KINDLE PW2/PW3 (GACH ENGINE)

Đối với hệ thống nhúng tài nguyên siêu hạn chế trên Kindle PW2/PW3 (CPU Cortex-A9 1.0 GHz, RAM khả dụng ~35-45MB), kiến trúc Clean Architecture được hiện thực hóa bằng C++17 theo các nguyên tắc sống còn sau:

### 8.1. ÁNH XẠ 4 TẦNG TRONG REPO C++17
1. **Domain Layer (`src/domain/`)**:
   - Chứa logic âm tiết thuần túy (`PhonicsSyllable`), máy trạng thái dòng chảy (`FlowFSM`), nhịp truyện (`StoryBeat`).
   - **Độc lập tuyệt đối**: CẤM import hoặc include bất kỳ header nào từ GTK3, Cairo, SQLite3, hoặc POSIX OS headers.
   - Chỉ sử dụng standard C++17 thuần túy (`<string_view>`, `<array>`, `<optional>`, `<variant>`, `<cstdint>`).
   - Mọi Value Objects phải nằm hoàn toàn trên **Stack** hoặc truyền qua `const &` với kích thước $le 64	ext{ bytes}$.
2. **Application Layer (`src/application/`)**:
   - Chứa Use Cases (`StoryInteractor`), điều phối sự kiện cảm ứng, chuyển nhịp truyện và gửi lệnh vẽ.
   - Inbound Ports (`IUserInputPort`): Giao diện nhận tín hiệu chạm hoặc sự kiện thời gian.
   - Outbound Ports (`IStoryRepository`, `IDisplayRenderer`): Giao diện trừu tượng hóa việc nạp dữ liệu và xuất hình ảnh ra màn hình E-Ink.
3. **Adapters Layer (`src/adapters/`)**:
   - `SqliteStoryRepository`: Triển khai `IStoryRepository` qua SQLite3 C-API với cache cứng 500KB.
   - `CairoRenderer`: Triển khai `IDisplayRenderer` vẽ trực tiếp lên cairo surface bằng cơ chế Dirty Rectangle partial refresh.
   - `GtkTouchAdapter`: Bắt tín hiệu chạm từ GTK3 chuyển đổi thành Use Case inputs.
4. **Infrastructure Layer (`src/infrastructure/`)**:
   - Quản trị vòng đời tiến trình, ngăn Kindle sleep qua LIPC, nạp cấu hình app, khởi chạy GTK main loop.

### 8.2. QUY TẮC PHỤ THUỘC & CẤP PHÁT BỘ NHỚ TRÊN THIẾT BỊ NHÚNG
- **Trần RAM toàn ứng dụng**: Khóa cứng trần bộ nhớ thường trú (RSS) $< 12	ext{ MB}$.
- **Zero-Dynamic-Allocation trong Hot Path**: Luồng render giao diện (`on_draw`) và luồng kiểm tra va chạm cảm ứng (`handle_touch`) tuyệt đối không dùng `new`/`malloc` hay tái phân bổ động.
- **Tách biệt nội dung khỏi mã nguồn (Content Decoupling)**: 100% kịch bản, lời dẫn, âm vị khuyết và cấu trúc trang truyện được lưu trong SQLite 3NF (`story_engine.db`), mã nguồn C++ đóng vai trò là Engine vạn năng không hardcode nội dung truyện.
