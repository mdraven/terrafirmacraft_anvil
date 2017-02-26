#pragma once

#include "Technique.hpp"

#include <array>
#include <functional>
#include <vector>

class Config;

class FindTechniques final {
public:
    using TechniqueChecker = std::function<bool(TechniqueType)>;
    using Pattern = std::array<TechniqueChecker, 3>;

    FindTechniques(const std::vector<Technique>& techniques, const Config& config);

    std::vector<std::vector<std::size_t>> find(const Pattern& pattern, unsigned int score) const;

private:
    const std::vector<Technique>& m_techniques;
    const Config& m_config;

    bool checkPatten(const Pattern& pattern, const std::vector<std::size_t>& chain) const;
    int calcScore(const std::vector<std::size_t>& chain) const;

    static void increment(std::vector<std::size_t>& vec, std::size_t mod);
};
