#pragma once

enum class TechniqueType {
    hit_light,
    hit_medium,
    hit_heavy,
    draw,
    punch,
    bend,
    upset,
    shrink
};

struct Technique final {
    TechniqueType m_type;
    int m_score = 0;
};

const char* get_name(TechniqueType type);
