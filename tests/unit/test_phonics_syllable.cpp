#include <cassert>
#include <iostream>
#include <string_view>
#include "domain/phonics_syllable.hpp"
#include "domain/domain_errors.hpp"

using namespace gach::domain;

void test_construct_valid_syllable() {
    PhonicsSyllable syl("m", "it", Tone::SAC, "mít");
    assert(syl.onset() == "m");
    assert(syl.rhyme() == "it");
    assert(syl.tone() == Tone::SAC);
    assert(syl.full_word() == "mít");
    std::cout << "  ✓ test_construct_valid_syllable passed" << std::endl;
}

void test_blending_matching() {
    // Trường hợp 1: Khuyết âm đầu (bé cần chọn 't' cho từ 'tưới')
    PhonicsSyllable syl1("t", "ươi", Tone::SAC, "tưới");
    assert(syl1.match_onset("t") == true);
    assert(syl1.match_onset("c") == false);
    assert(syl1.match_onset("b") == false);

    // Trường hợp 2: Khuyết vần (bé cần chọn 'ây' cho từ 'cây')
    PhonicsSyllable syl2("c", "ây", Tone::NGANG, "cây");
    assert(syl2.match_rhyme("ây") == true);
    assert(syl2.match_rhyme("an") == false);
    std::cout << "  ✓ test_blending_matching passed" << std::endl;
}

void test_vietnamese_tones() {
    PhonicsSyllable ba("b", "a", Tone::HUYEN, "bà");
    assert(ba.tone() == Tone::HUYEN);
    assert(ba.full_word() == "bà");

    PhonicsSyllable cay("c", "ây", Tone::NGANG, "cây");
    assert(cay.tone() == Tone::NGANG);

    PhonicsSyllable mit("m", "it", Tone::SAC, "mít");
    assert(mit.tone() == Tone::SAC);
    std::cout << "  ✓ test_vietnamese_tones passed" << std::endl;
}

void test_value_object_size_and_immutability() {
    // Khóa cứng kích thước Value Object <= 64 bytes để tối ưu Stack và bộ nhớ cache L1
    static_assert(sizeof(PhonicsSyllable) <= 64, "PhonicsSyllable must be <= 64 bytes for stack efficiency");

    PhonicsSyllable s1("m", "it", Tone::SAC, "mít");
    PhonicsSyllable s2("m", "it", Tone::SAC, "mít");
    PhonicsSyllable s3("b", "a", Tone::HUYEN, "bà");

    assert(s1 == s2);
    assert(!(s1 == s3));
    std::cout << "  ✓ test_value_object_size_and_immutability passed (" << sizeof(PhonicsSyllable) << " bytes)" << std::endl;
}

void test_invalid_construction_guard() {
    // Không cho phép từ rỗng
    bool caught = false;
    try {
        PhonicsSyllable invalid("", "", Tone::NGANG, "");
    } catch (const DomainValidationError& e) {
        caught = true;
    }
    assert(caught && "Must throw DomainValidationError when creating empty syllable");
    std::cout << "  ✓ test_invalid_construction_guard passed" << std::endl;
}

int main() {
    std::cout << "--- Running PhonicsSyllable Unit Tests ---" << std::endl;
    test_construct_valid_syllable();
    test_blending_matching();
    test_vietnamese_tones();
    test_value_object_size_and_immutability();
    test_invalid_construction_guard();
    std::cout << "All PhonicsSyllable tests passed!" << std::endl;
    return 0;
}
