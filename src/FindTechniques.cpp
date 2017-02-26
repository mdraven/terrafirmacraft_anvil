
#include "FindTechniques.hpp"

#include "Config.hpp"

FindTechniques::
FindTechniques(const std::vector<Technique>& techniques, const Config& config)
    : m_techniques(techniques)
    , m_config(config) {}

std::vector<std::vector<std::size_t>> FindTechniques::
find(const Pattern& pattern, unsigned int score) const {
    std::vector<std::vector<std::size_t>> result;

    std::vector<std::size_t> chain;

    while(true) {
        increment(chain, m_techniques.size());
        if(chain.size() > m_config.m_max_depth)
            break;
        // уже нашли цепочку короче данной
        if(!result.empty() && chain.size() > result.front().size())
            break;

        int current_score = calcScore(chain);
        if(current_score != score || !checkPatten(pattern, chain))
            continue;

        result.push_back(chain);
    }

    return result;
}

bool FindTechniques::
checkPatten(const Pattern& pattern, const std::vector<std::size_t>& chain) const {
    if(chain.size() < 3)
        return false;

    if(!pattern[0](m_techniques.at(chain[chain.size()-1]).m_type))
        return false;
    if(!pattern[1](m_techniques.at(chain[chain.size()-2]).m_type))
        return false;
    if(!pattern[2](m_techniques.at(chain[chain.size()-3]).m_type))
        return false;

    return true;
}

// 0, 1, 00, 01, 10, 11, 000, ...
// Возвращает все комбинации от самой короткой к самой длинной. Реализует обход
// дерева в ширину.
void FindTechniques::increment(std::vector<std::size_t>& vec, std::size_t mod) {
    if(vec.empty()) {
        vec.push_back(0);
        return;
    }

    for(auto it = vec.rbegin(); it != vec.rend(); ++it) {
        *it += 1;
        if (*it < mod)
            return;
        *it = 0;
    }

    vec.push_back(0);
}

// Если недопустимая комбинация, то возвращает -1
int FindTechniques::calcScore(const std::vector<std::size_t>& chain) const {
    int score = 0;

    for(std::size_t idx : chain) {
        score += m_techniques.at(idx).m_score;
        // при работе с наковальней очки не могут стать отрицательными или
        // больше максимума(это ломает предмет)
        if(score < 0 || score > m_config.m_max_score)
            return -1;
    }

    return score;
}
