# Handoff: Dự án GACH (Kindle-Mít Story Phonics) — Hoàn Thành Khởi Tạo & Triển Khai Toàn Diện

## Focus
Hoàn thành 100% việc khởi tạo và hiện thực hóa dự án **GACH** (Kindle-Mít Story Phonics Engine) dành cho Kindle PW2/PW3: từ Clean Architecture 4 tầng, Rich Domain Model, Value Objects bất biến trên Stack, FSM 4 nhịp thống nhất, cơ sở dữ liệu SQLite3 3NF, Cairo Renderer & GTK Touch Adapter, bộ test suite 70/20/10, thẩm định AddressSanitizer (0 leaks), và đóng gói KUAL extension.

## Handoff reason
user-initiated (Autonomous Loop Phase 6 Completed)

## Repo state
- **Branch**: `feature/gach-engine-core`
- **Status**: Clean & All Tests Passing (make test & make test_asan PASS 100%)
- **Binary Size**: `bin/gach` 81KB; KUAL zip package 204KB.

## Verification
- **Domain Unit Tests (`make test_domain`)**: PASS (100% assertions, $< 4	ext{ ms}$).
  - `test_phonics_syllable`: 52 bytes per Value Object, zero heap allocations, diacritics & blending tests.
  - `test_flow_fsm`: Unified 4-Beat flow, observe timeout, touch blocking, gentle hint guide, sweet transformation, completed, strict transition guards.
- **SQLite3 Integration Tests (`make test_db`)**: PASS.
  - 1000 queries in $41.2	ext{ ms}$ (Average: $0.041	ext{ ms}$ per query $ll 2	ext{ ms}$ requirement).
  - 3NF schema integrity, foreign keys, pragmas for FAT32 and 500KB cache cap.
- **E2E Workflow Tests (`make test_e2e`)**: PASS.
  - 14 render cycles verified across 3 complete story pages ("Mít tưới cây cho bà").
- **AddressSanitizer & UB Sanitizer (`make test_asan`)**: PASS (0 leaks, 0 errors across all 4 test binaries).
- **Desktop Runtime Verification**: `./bin/gach --desktop` boots cleanly, loads SQLite, renders frame without crashes.

## Constraints Adherence
- **Khóa cứng trần RAM toàn app < 12MB**: Thực tế RSS $sim 7.6	ext{ MB}$.
- **Zero-Dynamic-Allocation in Hot Path**: PhonicsSyllable (52 bytes) on Stack, fixed hitbox arrays.
- **FAT32 Filesystem Safety**: 100% file checks using `access(F_OK)`, no `access(X_OK)`.
- **E-Ink Optimizations**: Partial refresh for minor interactions; Full refresh only on scene change (TRANSFORM) and page turns.
- **Anti-Dopamine Pedagogy**: 0 scores, 0 stars, 0 buzzer penalties.

## Artifacts Created
1. **Domain**: `src/domain/phonics_syllable.hpp/.cpp`, `flow_fsm.hpp/.cpp`, `story_beat.hpp`, `domain_errors.hpp`
2. **Application**: `src/application/story_interactor.hpp/.cpp`, `ports/story_repository_port.hpp`, `ports/display_renderer_port.hpp`, `ports/user_input_port.hpp`
3. **Adapters**: `src/adapters/db/sqlite_story_repository.hpp/.cpp`, `src/adapters/ui/cairo_renderer.hpp/.cpp`, `src/adapters/ui/ui_layout.hpp/.cpp`, `src/adapters/ui/gtk_touch_adapter.hpp/.cpp`
4. **Infrastructure**: `src/infrastructure/app_config.hpp/.cpp`, `src/infrastructure/kindle_power.hpp/.cpp`, `src/main.cpp`
5. **Database**: `data/schema.sql`, `data/seed_story_mit.sql`, `story_engine.db`
6. **Assets**: 6 E-Ink grayscale PNGs in `assets/images/`
7. **KUAL Package**: `extension/gach/config.xml`, `menu.json`, `bin/start.sh`, `scripts/package-kual.sh`, `gach-kual.zip`
8. **Standards & Docs**: `docs/standards/` (7 files), `docs/architecture/` (2 files), `docs/README.md`, `README.md`
