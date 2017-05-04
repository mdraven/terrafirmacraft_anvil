
#include "Rule.hpp"

#include <cassert>

static const std::size_t g_max_value = 2;

const std::vector<Rule> g_rules{{
    {"Any", RuleType::any, 0, 2},

    {"Hit any", RuleType::hit, 0, 2},
    {"Hit last", RuleType::hit, 0, 0},
    {"Hit second from last", RuleType::hit, 1, 1},
    {"Hit third from last", RuleType::hit, 2, 2},
    {"Hit last two", RuleType::hit, 0, 1},
    {"Hit not last", RuleType::hit, 1, 2},

    {"Draw any", RuleType::draw, 0, 2},
    {"Draw last", RuleType::draw, 0, 0},
    {"Draw second from last", RuleType::draw, 1, 1},
    {"Draw third from last", RuleType::draw, 2, 2},
    {"Draw last two", RuleType::draw, 0, 1},
    {"Draw not last", RuleType::draw, 1, 2},

    {"Punch any", RuleType::punch, 0, 2},
    {"Punch last", RuleType::punch, 0, 0},
    {"Punch second from last", RuleType::punch, 1, 1},
    {"Punch third from last", RuleType::punch, 2, 2},
    {"Punch last two", RuleType::punch, 0, 1},
    {"Punch not last", RuleType::punch, 1, 2},

    {"Shrink any", RuleType::shrink, 0, 2},
    {"Shrink last", RuleType::shrink, 0, 0},
    {"Shrink second from last", RuleType::shrink, 1, 1},
    {"Shrink third from last", RuleType::shrink, 2, 2},
    {"Shrink last two", RuleType::shrink, 0, 1},
    {"Shrink not last", RuleType::shrink, 1, 2},

    {"Upset any", RuleType::upset, 0, 2},
    {"Upset last", RuleType::upset, 0, 0},
    {"Upset second from last", RuleType::upset, 1, 1},
    {"Upset third from last", RuleType::upset, 2, 2},
    {"Upset last two", RuleType::upset, 0, 1},
    {"Upset not last", RuleType::upset, 1, 2},

    {"Bend any", RuleType::bend, 0, 2},
    {"Bend last", RuleType::bend, 0, 0},
    {"Bend second from last", RuleType::bend, 1, 1},
    {"Bend third from last", RuleType::bend, 2, 2},
    {"Bend last two", RuleType::bend, 0, 1},
    {"Bend not last", RuleType::bend, 1, 2},
}};

Rule::Rule(const std::string& name, RuleType type, unsigned int min, unsigned int max)
    : m_name(name)
    , m_type(type)
    , m_min(min)
    , m_max(max)
{}

bool Rule::check(const std::vector<TechniqueType>& techniques) const {
    assert(techniques.size() <= 3);

    if(m_type == RuleType::any)
        return true;

    if(techniques.size() == 0)
        return false;

    for(std::size_t i = m_min; i <= std::min<std::size_t>(m_max, techniques.size() - 1); ++i) {
        if(m_type == RuleType::hit) {
            if(techniques[i] == TechniqueType::hit_light
               || techniques[i] == TechniqueType::hit_medium
               || techniques[i] == TechniqueType::hit_heavy)
                return true;
        }
        if(m_type == RuleType::punch) {
            if(techniques[i] == TechniqueType::punch)
                return true;
        }
        if(m_type == RuleType::shrink) {
            if(techniques[i] == TechniqueType::shrink)
                return true;
        }
        if(m_type == RuleType::draw) {
            if(techniques[i] == TechniqueType::draw)
                return true;
        }
        if(m_type == RuleType::upset) {
            if(techniques[i] == TechniqueType::upset)
                return true;
        }
        if(m_type == RuleType::bend) {
            if(techniques[i] == TechniqueType::bend)
                return true;
        }
    }

    return false;
}

RuleType Rule::getType() const {
    return m_type;
}

const std::string& Rule::getName() const {
    return m_name;
}

unsigned int Rule::getMax() const {
    return m_max;
}
