#pragma once

#include <array>
#include <cstdint>
#include <cstring>
#include <string>
#include <string_view>
#include "domain/phonics_syllable.hpp"

namespace gach::domain {

enum class MissingPart : uint8_t {
    ONSET = 0, // Khuyết âm đầu (e.g. [t] - ưới)
    RHYME = 1  // Khuyết vần (e.g. c - [ây])
};

struct ChoiceItem {
    std::array<char, 16> text{};
    uint8_t len{0};
    bool is_correct{false};

    ChoiceItem() = default;
    ChoiceItem(std::string_view str, bool correct) : is_correct(correct) {
        len = static_cast<uint8_t>(str.size() < 16 ? str.size() : 15);
        std::memcpy(text.data(), str.data(), len);
        text[len] = 0;
    }

    [[nodiscard]] std::string_view display_text() const noexcept {
        return std::string_view(text.data(), len);
    }
};

class StoryBeat {
public:
    StoryBeat() = default;
    StoryBeat(uint32_t page_num,
              std::string lead_text,
              std::string initial_img,
              std::string transform_img,
              PhonicsSyllable syllable,
              MissingPart missing_part,
              const std::array<ChoiceItem, 4>& choices,
              uint8_t choice_count)
        : m_page_number(page_num),
          m_lead_text(std::move(lead_text)),
          m_initial_image_path(std::move(initial_img)),
          m_transform_image_path(std::move(transform_img)),
          m_syllable(syllable),
          m_missing_part(missing_part),
          m_choices(choices),
          m_choice_count(choice_count) {}

    [[nodiscard]] uint32_t page_number() const noexcept { return m_page_number; }
    [[nodiscard]] const std::string& lead_text() const noexcept { return m_lead_text; }
    [[nodiscard]] const std::string& initial_image_path() const noexcept { return m_initial_image_path; }
    [[nodiscard]] const std::string& transform_image_path() const noexcept { return m_transform_image_path; }
    [[nodiscard]] const PhonicsSyllable& syllable() const noexcept { return m_syllable; }
    [[nodiscard]] MissingPart missing_part() const noexcept { return m_missing_part; }
    [[nodiscard]] const std::array<ChoiceItem, 4>& choices() const noexcept { return m_choices; }
    [[nodiscard]] uint8_t choice_count() const noexcept { return m_choice_count; }

private:
    uint32_t m_page_number{1};
    std::string m_lead_text{};
    std::string m_initial_image_path{};
    std::string m_transform_image_path{};
    PhonicsSyllable m_syllable{"m", "it", Tone::SAC, "mít"};
    MissingPart m_missing_part{MissingPart::ONSET};
    std::array<ChoiceItem, 4> m_choices{};
    uint8_t m_choice_count{0};
};

} // namespace gach::domain