#pragma once

#include <cstdint>
#include <optional>
#include <string_view>
#include "domain/story_beat.hpp"

namespace gach::application::ports {

/**
 * @brief Outbound Port trừu tượng hóa cơ chế lưu trữ kịch bản truyện và âm vị.
 * Độc lập hoàn toàn với công nghệ SQLite / JSON / Flash.
 */
class IStoryRepository {
public:
    virtual ~IStoryRepository() = default;

    virtual bool load_story(std::string_view story_id) = 0;
    [[nodiscard]] virtual uint32_t total_pages() const noexcept = 0;
    [[nodiscard]] virtual std::string_view current_story_id() const noexcept = 0;
    [[nodiscard]] virtual std::string_view story_title() const noexcept = 0;
    [[nodiscard]] virtual std::optional<domain::StoryBeat> get_beat(uint32_t page_number) = 0;
};

} // namespace gach::application::ports