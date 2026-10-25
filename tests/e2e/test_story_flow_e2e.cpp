#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include "adapters/db/sqlite_story_repository.hpp"
#include "application/ports/display_renderer_port.hpp"
#include "application/story_interactor.hpp"

using namespace gach;
using namespace gach::domain;

class SpyDisplayRenderer : public application::ports::IDisplayRenderer {
public:
    int render_calls{0};
    int full_refresh_count{0};
    int partial_refresh_count{0};
    BeatState last_state{BeatState::OBSERVE};
    uint32_t last_page{0};

    void render_beat(const StoryBeat& beat, const FlowFSM& fsm, bool full_refresh) override {
        render_calls++;
        if (full_refresh) full_refresh_count++;
        else partial_refresh_count++;
        last_state = fsm.state();
        last_page = beat.page_number();
    }

    void request_redraw(bool full_refresh) override {
        if (full_refresh) full_refresh_count++;
        else partial_refresh_count++;
    }

    void set_resolution([[maybe_unused]] int width, [[maybe_unused]] int height) override {}
};

std::string read_file_e2e(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) throw std::runtime_error("Cannot open " + path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void test_full_story_flow_e2e() {
    adapters::db::SqliteStoryRepository repo(":memory:");
    assert(repo.initialize() == true);
    assert(repo.execute_sql(read_file_e2e("data/schema.sql")));
    assert(repo.execute_sql(read_file_e2e("data/seed_story_mit.sql")));

    SpyDisplayRenderer renderer;
    application::StoryInteractor interactor(repo, renderer);

    // 1. Bắt đầu truyện 'Mít tưới cây cho bà'
    assert(interactor.start_story("mit_tuoi_cay") == true);
    assert(interactor.current_page() == 1);
    assert(interactor.fsm().state() == BeatState::OBSERVE);
    assert(interactor.fsm().is_touch_locked() == true);
    assert(renderer.render_calls == 1);
    assert(renderer.full_refresh_count == 1);

    // 2. Chạm trong nhịp OBSERVE -> Phải bị chặn hoàn toàn
    auto choice0_rect = interactor.layout().choice_rect(0);
    interactor.on_touch(choice0_rect.x + 10, choice0_rect.y + 10);
    assert(interactor.fsm().state() == BeatState::OBSERVE);

    // 3. Tiến trình thời gian 2.5s (2500ms) quan sát tranh
    interactor.on_timer_tick(2500);
    assert(interactor.fsm().state() == BeatState::AWAIT_INPUT);
    assert(interactor.fsm().is_touch_locked() == false);
    assert(renderer.partial_refresh_count == 1);

    // 4. Bé chạm lựa chọn SAI (Choice index 1: 'c')
    auto choice1_rect = interactor.layout().choice_rect(1);
    interactor.on_touch(choice1_rect.x + 10, choice1_rect.y + 10);
    assert(interactor.fsm().state() == BeatState::AWAIT_INPUT);
    assert(interactor.fsm().incorrect_attempts() == 1);

    // 5. Sau 6 giây không tương tác tiếp -> Kích hoạt hint dẫn lối
    interactor.on_timer_tick(6000);
    assert(interactor.fsm().hint_visible() == true);

    // 6. Bé chạm lựa chọn ĐÚNG (Choice index 0: 't')
    interactor.on_touch(choice0_rect.x + 10, choice0_rect.y + 10);
    assert(interactor.fsm().state() == BeatState::TRANSFORM);
    assert(interactor.fsm().is_touch_locked() == true);
    assert(renderer.last_state == BeatState::TRANSFORM);
    assert(renderer.full_refresh_count == 2); // Đổi tranh quả ngọt: Full refresh

    // 7. Khoảng lặng chuyển hóa 2.0s kết thúc -> Sang nhịp COMPLETED
    interactor.on_timer_tick(2000);
    assert(interactor.fsm().state() == BeatState::COMPLETED);
    assert(interactor.fsm().is_touch_locked() == false);

    // 8. Bé bấm nút mũi tên sang Trang 2
    auto next_rect = interactor.layout().next_button_rect();
    interactor.on_touch(next_rect.x + 10, next_rect.y + 10);
    assert(interactor.current_page() == 2);
    assert(interactor.fsm().state() == BeatState::OBSERVE);
    assert(interactor.current_beat()->syllable().full_word() == "cây");

    // 9. Hoàn thành trang 2
    interactor.on_timer_tick(2500); // hết observe
    assert(interactor.fsm().state() == BeatState::AWAIT_INPUT);
    interactor.on_touch(choice0_rect.x + 10, choice0_rect.y + 10); // chọn đúng 'ây'
    assert(interactor.fsm().state() == BeatState::TRANSFORM);
    interactor.on_timer_tick(2000); // hết transform
    assert(interactor.fsm().state() == BeatState::COMPLETED);
    interactor.on_touch(next_rect.x + 10, next_rect.y + 10); // sang trang 3
    assert(interactor.current_page() == 3);
    assert(interactor.current_beat()->syllable().full_word() == "bà");

    // 10. Hoàn thành trang 3 (Trang cuối)
    interactor.on_timer_tick(2500);
    interactor.on_touch(choice0_rect.x + 10, choice0_rect.y + 10); // chọn đúng 'b'
    interactor.on_timer_tick(2000);
    assert(interactor.fsm().state() == BeatState::COMPLETED);
    // Chạm next khi ở trang cuối -> không bị crash hay tràn số trang
    assert(interactor.advance_page() == false);
    assert(interactor.current_page() == 3);

    std::cout << "  ✓ test_full_story_flow_e2e passed with " << renderer.render_calls << " total renders" << std::endl;
}

int main() {
    std::cout << "--- Running End-to-End Story Flow Tests ---" << std::endl;
    test_full_story_flow_e2e();
    std::cout << "All E2E Tests passed!" << std::endl;
    return 0;
}