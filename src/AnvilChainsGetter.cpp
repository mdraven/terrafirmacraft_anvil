
#include "AnvilChainsGetter.hpp"

#include "anvil_chains.hpp"
#include "Rule.hpp"

// максимальное число очков; при переполнении этого значения заготовка исчезает
static const std::size_t g_max_score = 150;

std::vector<std::array<TechniqueType, 3>> AnvilChainsGetter::
getTechniques(const std::array<std::size_t, 3>& pattern) {
    std::vector<std::array<TechniqueType, 3>> result;

    for(std::size_t i = 0; i < static_cast<std::size_t>(TechniqueType::last); ++i) {
        for(std::size_t j = 0; j < static_cast<std::size_t>(TechniqueType::last); ++j)
            for(std::size_t k = 0; k < static_cast<std::size_t>(TechniqueType::last); ++k) {
                std::array<TechniqueType, 3> techniques{
                    static_cast<TechniqueType>(i),
                    static_cast<TechniqueType>(j),
                    static_cast<TechniqueType>(k)
                };

                if(g_rules.at(pattern[0]).check(techniques) &&
                   g_rules.at(pattern[1]).check(techniques) &&
                   g_rules.at(pattern[2]).check(techniques))
                    result.push_back(techniques);
            }
    }

    return result;
}

std::size_t AnvilChainsGetter::countChanges(const std::vector<std::uint8_t>& chain) {
    if (chain.empty())
        return 0;

    std::size_t changes = 0;
    std::uint8_t last = chain[0];

    for(std::size_t i = 1; i < chain.size(); ++i) {
        if(chain[i] != last)
            ++changes;
        last = chain[i];
    }

    return changes;
}

std::vector<std::vector<std::uint8_t>> AnvilChainsGetter::
getShorts(const std::vector<std::vector<std::uint8_t>>& chains) {
    std::vector<std::vector<std::uint8_t>> short_chains;
    std::size_t min_length = 9999;

    for(const auto& chain : chains)
        min_length = std::min(chain.size(), min_length);

    for(const auto& chain : chains) {
        if(chain.size() == min_length)
            short_chains.push_back(chain);
    }

    return short_chains;
}

std::vector<std::vector<std::uint8_t>> AnvilChainsGetter::
getLessChanges(const std::vector<std::vector<std::uint8_t>>& chains) {
    std::vector<std::vector<std::uint8_t>> less_change_chains;

    std::size_t num_changes = 9999;
    for(const auto& chain : chains)
        num_changes = std::min(countChanges(chain), num_changes);

    for(const auto& chain : chains) {
        if(countChanges(chain) == num_changes)
            less_change_chains.push_back(chain);
    }

    return less_change_chains;
}

std::vector<std::uint8_t> AnvilChainsGetter::
get(const std::array<std::size_t, 3>& pattern, unsigned char score) {
    if(score > g_max_score)
        return std::vector<std::uint8_t>();

    const std::vector<std::array<TechniqueType, 3>> techniques = getTechniques(pattern);

    std::vector<std::vector<std::uint8_t>> chains;
    for(auto& technique : techniques) {
        int score1 = g_techniques.at(static_cast<std::size_t>(technique[0])).m_score;
        int score2 = g_techniques.at(static_cast<std::size_t>(technique[1])).m_score;
        int score3 = g_techniques.at(static_cast<std::size_t>(technique[2])).m_score;

        int tmp_score = score - score1 - score2 - score3;
        if(tmp_score < 0 || tmp_score > g_max_score)
            continue;

        std::vector<std::vector<std::uint8_t>> tmp_chains = g_anvil_chains.at(tmp_score);
        for(auto& chain : tmp_chains) {
            chain.push_back(static_cast<std::uint8_t>(technique[2]));
            chain.push_back(static_cast<std::uint8_t>(technique[1]));
            chain.push_back(static_cast<std::uint8_t>(technique[0]));
        }

        chains.insert(chains.begin(), tmp_chains.begin(), tmp_chains.end());
    }

    if(chains.empty())
        return std::vector<std::uint8_t>();

    // так как я не придумал как выбрать из оставшихся цепей лучшую,
    // то выбиру первую
    return getLessChanges(getShorts(chains)).front();
}
