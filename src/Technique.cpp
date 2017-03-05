
#include "Technique.hpp"

Technique::Technique(TechniqueType type, int score, const std::string& name, bool red)
    : m_type(type)
    , m_score(score)
    , m_name(name)
    , m_red(red) {}

const std::array<Technique, to_under(TechniqueType::last)> g_techniques{{
    Technique{TechniqueType::hit_light, -3, "r1", true},
    Technique{TechniqueType::hit_medium, -6, "r2", true},
    Technique{TechniqueType::hit_heavy, -9, "r3", true},
    Technique{TechniqueType::draw, -15, "r4", true},

    Technique{TechniqueType::punch, 2, "g1", false},
    Technique{TechniqueType::bend, 7, "g2", false},
    Technique{TechniqueType::upset, 13, "g3", false},
    Technique{TechniqueType::shrink, 16, "g4", false}
}};
