#include "domain/phonics_syllable.hpp"
#include <cstring>

namespace gach::domain {

PhonicsSyllable::PhonicsSyllable(std::string_view onset, std::string_view rhyme, Tone tone, std::string_view full_word)
    : m_tone(tone) {
    if (full_word.empty()) {
        throw DomainValidationError("PhonicsSyllable full_word cannot be empty");
    }
    if (onset.size() >= m_onset.size()) {
        throw DomainValidationError("Onset exceeds fixed buffer limit (8 bytes)");
    }
    if (rhyme.size() >= m_rhyme.size()) {
        throw DomainValidationError("Rhyme exceeds fixed buffer limit (16 bytes)");
    }
    if (full_word.size() >= m_full_word.size()) {
        throw DomainValidationError("Full word exceeds fixed buffer limit (24 bytes)");
    }

    m_onset_len = static_cast<uint8_t>(onset.size());
    std::memcpy(m_onset.data(), onset.data(), onset.size());
    m_onset[m_onset_len] = 0;

    m_rhyme_len = static_cast<uint8_t>(rhyme.size());
    std::memcpy(m_rhyme.data(), rhyme.data(), rhyme.size());
    m_rhyme[m_rhyme_len] = 0;

    m_word_len = static_cast<uint8_t>(full_word.size());
    std::memcpy(m_full_word.data(), full_word.data(), full_word.size());
    m_full_word[m_word_len] = 0;
}

std::string_view PhonicsSyllable::onset() const noexcept {
    return std::string_view(m_onset.data(), m_onset_len);
}

std::string_view PhonicsSyllable::rhyme() const noexcept {
    return std::string_view(m_rhyme.data(), m_rhyme_len);
}

Tone PhonicsSyllable::tone() const noexcept {
    return m_tone;
}

std::string_view PhonicsSyllable::full_word() const noexcept {
    return std::string_view(m_full_word.data(), m_word_len);
}

bool PhonicsSyllable::match_onset(std::string_view candidate) const noexcept {
    return onset() == candidate;
}

bool PhonicsSyllable::match_rhyme(std::string_view candidate) const noexcept {
    return rhyme() == candidate;
}

bool PhonicsSyllable::operator==(const PhonicsSyllable& other) const noexcept {
    return m_tone == other.m_tone &&
           onset() == other.onset() &&
           rhyme() == other.rhyme() &&
           full_word() == other.full_word();
}

bool PhonicsSyllable::operator!=(const PhonicsSyllable& other) const noexcept {
    return !(*this == other);
}

} // namespace gach::domain