
#include "Technique.hpp"

const char* get_name(TechniqueType type) {
    switch(type) {
    case TechniqueType::hit_light:
        return "r1";
    case TechniqueType::hit_medium:
        return "r2";
    case TechniqueType::hit_heavy:
        return "r3";
    case TechniqueType::draw:
        return "r4";
    case TechniqueType::punch:
        return "g1";
    case TechniqueType::bend:
        return "g2";
    case TechniqueType::upset:
        return "g3";
    case TechniqueType::shrink:
        return "g4";
    }

    return "unknown";
}
