
#include "Technique.hpp"

const std::array<Technique, to_under(TechniqueType::last)> g_techniques{{
    {TechniqueType::hit_light, -3, "r1", true},
    {TechniqueType::hit_medium, -6, "r2", true},
    {TechniqueType::hit_heavy, -9, "r3", true},
    {TechniqueType::draw, -15, "r4", true},

    {TechniqueType::punch, 2, "g1", false},
    {TechniqueType::bend, 7, "g2", false},
    {TechniqueType::upset, 13, "g3", false},
    {TechniqueType::shrink, 16, "g4", false}
}};
