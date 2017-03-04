#pragma once

#include "Technique.hpp"

#include <array>
#include <string>
#include <vector>

enum class RuleType : std::uint8_t {
    any,
    hit,
    punch,
    shrink,
    draw,
    upset,
    bend,

    last
};

// Элемент кузнечного паттерна
class Rule {
    std::string m_name;
    RuleType m_type;
    unsigned int m_min = 0;
    unsigned int m_max = 0;

public:
    Rule() = default;
    Rule(const std::string& name, RuleType type, unsigned int min, unsigned int max);

    bool check(const std::array<TechniqueType, 3>& techniques) const;

    RuleType getType() const;
    const std::string& getName() const;
};

extern const std::vector<Rule> g_rules;
