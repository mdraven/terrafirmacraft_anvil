
#include <array>
#include <functional>
#include <vector>

#include <iostream>
#include <stdexcept>

struct Config final {
    unsigned int m_max_score = 150;
    unsigned int m_max_depth = 20;
};

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

using TechniqueChecker = std::function<bool(TechniqueType)>;
using Pattern = std::array<TechniqueChecker, 3>;

// 0, 1, 00, 01, 10, 11, 000, ...
// Возвращает все комбинации от самой короткой к самой длинной. Реализует обход
// дерева в ширину.
void increment(std::vector<std::size_t>& vec, std::size_t mod) {
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
int calc_score(const std::vector<Technique>& techniques, const Config& config, const std::vector<std::size_t>& chain) {
    int score = 0;

    for(std::size_t idx : chain) {
        score += techniques.at(idx).m_score;
        // при работе с наковальней очки не могут стать отрицательными или
        // больше максимума(это ломает предмет)
        if(score < 0 || score > config.m_max_score)
            return -1;
    }

    return score;
}

bool check_patten(const std::vector<Technique>& techniques, const Pattern& pattern,
                  const std::vector<std::size_t>& chain) {
    if(chain.size() < 3)
        return false;

    if(!pattern[0](techniques.at(chain[chain.size()-1]).m_type))
        return false;
    if(!pattern[1](techniques.at(chain[chain.size()-2]).m_type))
        return false;
    if(!pattern[2](techniques.at(chain[chain.size()-3]).m_type))
        return false;

    return true;
}

std::vector<std::vector<std::size_t>> find_techniques(const std::vector<Technique>& techniques, const Config& config,
                                                      const Pattern& pattern, unsigned int score) {
    std::vector<std::vector<std::size_t>> result;

    std::vector<std::size_t> chain;

    while(true) {
        increment(chain, techniques.size());
        if(chain.size() > config.m_max_depth)
            break;
        // уже нашли цепочку короче данной
        if(!result.empty() && chain.size() > result.front().size())
            break;

        int current_score = calc_score(techniques, config, chain);
        if(current_score != score || !check_patten(techniques, pattern, chain))
            continue;

        result.push_back(chain);
    }

    return result;
}

void print_chain(const std::vector<Technique>& techniques, const std::vector<std::size_t>& chain) {
    for(std::size_t idx : chain) {
        const char* str = nullptr;

        switch(techniques.at(idx).m_type) {
        case TechniqueType::hit_light:
            str = "r1"; break;
        case TechniqueType::hit_medium:
            str = "r2"; break;
        case TechniqueType::hit_heavy:
            str = "r3"; break;
        case TechniqueType::draw:
            str = "r4"; break;
        case TechniqueType::punch:
            str = "g1"; break;
        case TechniqueType::bend:
            str = "g2"; break;
        case TechniqueType::upset:
            str = "g3"; break;
        case TechniqueType::shrink:
            str = "g4"; break;
        }

        std::cout << str << " ";
    }

    std::cout << std::endl;
}


int main(int argc, char *argv[]) {
    try {
        const std::vector<Technique> hits{
            {TechniqueType::hit_light, -3},
            {TechniqueType::hit_medium, -6},
            {TechniqueType::hit_heavy, -9},
            {TechniqueType::draw, -15},

            {TechniqueType::punch, 2},
            {TechniqueType::bend, 7},
            {TechniqueType::upset, 13},
            {TechniqueType::shrink, 16},
        };

        auto any_hammer = [](TechniqueType type) {
                              return type == TechniqueType::hit_light ||
                                  type == TechniqueType::hit_medium ||
                                  type == TechniqueType::hit_heavy;
                          };
        auto g3 = [](TechniqueType type) {
                      return type == TechniqueType::upset;
                  };

        auto results = find_techniques(hits, Config(), {any_hammer, any_hammer, g3}, 238/2);

        for(auto& result : results)
            print_chain(hits, result);
    }
    catch(const std::exception& e) {
        std::cout << "Exception was catched in main(): " << e.what() << std::endl;
        throw;
    }

    return 0;
}
