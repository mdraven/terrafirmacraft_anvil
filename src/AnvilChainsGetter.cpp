
#include "AnvilChainsGetter.hpp"

#include "anvil_chains.hpp"
#include "increment.hpp"
#include "Rule.hpp"

#include <numeric>

// максимальное число очков; при переполнении этого значения заготовка исчезает
static const int g_max_score = 150;

std::vector<std::vector<TechniqueType>> AnvilChainsGetter::
getTechniques(const std::array<std::size_t, 3>& pattern) {
    std::vector<std::vector<TechniqueType>> result;

    std::vector<std::size_t> chain;

    while(true) {
        increment(chain, g_techniques.size());
        if(chain.size() > 3)
            break;

        std::vector<TechniqueType> techniques;
        for(std::size_t elem : chain)
            techniques.push_back(static_cast<TechniqueType>(elem));

        if(g_rules.at(pattern[0]).check(techniques) &&
           g_rules.at(pattern[1]).check(techniques) &&
           g_rules.at(pattern[2]).check(techniques))
               result.push_back(techniques);
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

std::vector<std::vector<std::uint8_t>> AnvilChainsGetter::
getIntoScaleLimits(int end_score, const std::vector<std::vector<std::uint8_t>>& chains) {
    std::vector<std::vector<std::uint8_t>> result;

    for(auto& chain : chains) {
        int tmp_score = end_score;

        for(auto& technique : chain) {
            tmp_score -= g_techniques.at(static_cast<std::size_t>(technique)).m_score;
            if(!checkScaleLimits(tmp_score))
                goto next_chain;
        }

        result.push_back(chain);

    next_chain:;
    }

    return result;
}

bool AnvilChainsGetter::checkScaleLimits(int score) {
    return (score >= 0 && score <= g_max_score);
}

int AnvilChainsGetter::getScoreBefore(int end_score, const std::vector<TechniqueType>& techniques) {
    for(auto& technique : techniques) {
        int score = g_techniques.at(static_cast<std::size_t>(technique)).m_score;

        end_score -= score;

        if(!checkScaleLimits(end_score))
            return -1;
    }

    return end_score;
}

std::vector<std::uint8_t> AnvilChainsGetter::
get(const std::array<std::size_t, 3>& pattern, int begin_score, int end_score) {
    if(!checkScaleLimits(begin_score))
        return std::vector<std::uint8_t>();

    if(!checkScaleLimits(end_score))
        return std::vector<std::uint8_t>();

    const std::vector<std::vector<TechniqueType>> techniques = getTechniques(pattern);

    std::vector<std::vector<std::uint8_t>> chains;
    for(auto& technique : techniques) {
        int score_before = getScoreBefore(end_score, technique);
        if(score_before == -1)
            continue;

        std::vector<std::vector<std::uint8_t>> tmp_chains =
            getIntoScaleLimits(score_before, g_anvil_chains.at(g_max_score + score_before - begin_score));

        std::vector<std::uint8_t> reverse_technique;
        for(auto it = technique.rbegin(), itend = technique.rend(); it != itend; ++it)
            reverse_technique.push_back(static_cast<std::uint8_t>(*it));

        for(auto& chain : tmp_chains)
            chain.insert(chain.end(), reverse_technique.begin(), reverse_technique.end());

        chains.insert(chains.begin(), tmp_chains.begin(), tmp_chains.end());
    }

    if(chains.empty())
        return std::vector<std::uint8_t>();

    // так как я не придумал как выбрать из оставшихся цепей лучшую,
    // то выбиру первую
    return getLessChanges(getShorts(chains)).front();
}
