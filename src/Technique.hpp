#pragma once

#include <array>

enum class TechniqueType {
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
};

extern const std::array<Technique, static_cast<std::size_t>(TechniqueType::last)> g_techniques;
