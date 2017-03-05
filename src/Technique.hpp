#pragma once

#include "utils.hpp"

#include <array>

enum class TechniqueType : std::uint8_t {
    hit_light = 0,
    hit_medium,
    hit_heavy,
    draw,
    punch,
    bend,
    upset,
    shrink,

    last
};

struct Technique final {
    TechniqueType m_type;
    int m_score = 0;
    std::string m_name;
    bool m_red;

    Technique(TechniqueType type, int score, const std::string& name, bool red);
};

extern const std::array<Technique, to_under(TechniqueType::last)> g_techniques;
