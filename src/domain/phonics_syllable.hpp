#pragma once

#include <array>
#include <cstdint>
#include <string_view>
#include "domain/domain_errors.hpp"

namespace gach::domain {

enum class Tone : uint8_t {
    NGANG = 0,
    HUYEN = 1,
    SAC   = 2,
    HOI   = 3,
    NGA   = 4,
    NANG  = 5
};

class PhonicsSyllable {
public:
    PhonicsSyllable(std::string_view onset, std::string_view rhyme, Tone tone, std::string_view full_word);

    [[nodiscard]] std::string_view onset() const noexcept;
    [[nodiscard]] std::string_view rhyme() const noexcept;
    [[nodiscard]] Tone tone() const noexcept;
    [[nodiscard]] std::string_view full_word() const noexcept;

    [[nodiscard]] bool match_onset(std::string_view candidate) const noexcept;
    [[nodiscard]] bool match_rhyme(std::string_view candidate) const noexcept;

    bool operator==(const PhonicsSyllable& other) const noexcept;
    bool operator!=(const PhonicsSyllable& other) const noexcept;

private:
    std::array<char, 8> m_onset{};
    std::array<char, 16> m_rhyme{};
    std::array<char, 24> m_full_word{};
    uint8_t m_onset_len{0};
    uint8_t m_rhyme_len{0};
    uint8_t m_word_len{0};
    Tone m_tone{Tone::NGANG};
};

} // namespace gach::domain