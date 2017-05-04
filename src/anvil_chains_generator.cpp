
#include "Technique.hpp"

#include "increment.hpp"

#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <vector>

// максимальная глубина поиска
static const std::size_t g_max_depth = 15;

// максимальное число очков; при переполнении этого значения заготовка исчезает
static const int g_max_score = 150;

void print_chain(const std::vector<std::size_t>& chain) {
    for(std::size_t idx : chain)
        std::cout << g_techniques.at(idx).m_name << " ";

    std::cout << std::endl;
}

// считает количество смен техник
// aabccd -- 4 смены
std::size_t count_changes(const std::vector<std::size_t>& chain) {
    if (chain.empty())
        return 0;

    std::size_t changes = 0;
    std::size_t last = chain[0];

    for(std::size_t i = 1; i < chain.size(); ++i) {
        if(chain[i] != last)
            ++changes;
        last = chain[i];
    }

    return changes;
}

// Суммирует очки цепочки
int calc_score(const std::vector<std::size_t>& chain) {
    int score = 0;

    for(std::size_t idx : chain)
        score += g_techniques.at(idx).m_score;

    return score;
}

std::vector<std::vector<std::size_t>> find(int begin_score, int end_score) {
    std::vector<std::vector<std::size_t>> result;

    std::vector<std::size_t> chain;

    while(true) {
        increment(chain, g_techniques.size());
        if(chain.size() > g_max_depth)
            break;
        // уже нашли цепочку короче данной
        if(!result.empty() && chain.size() > result.front().size())
            break;

        int current_score = calc_score(chain);

        // при работе с наковальней очки не могут стать отрицательными или
        // больше максимума(это ломает предмет)
        int absolute_score = current_score + begin_score;
        if(absolute_score < 0 || absolute_score > g_max_score)
            continue;

        int score = end_score - begin_score;
        if(current_score != score)
            continue;

        result.push_back(chain);
    }

    return result;
}

// Оставляет только те цепочки, где меньше всего смен техник.
std::vector<std::vector<std::size_t>> filter_minimal_changed(const std::vector<std::vector<std::size_t>>& chains) {
    std::size_t min_changes = 9999;
    for(auto& chain : chains)
        min_changes = std::min(min_changes, count_changes(chain));

    std::vector<std::vector<std::size_t>> result;
    for(auto& chain : chains)
        if(count_changes(chain) == min_changes)
            result.push_back(chain);

    return result;
}

void print_for_human(const std::map<int, std::set<std::vector<std::size_t>>>& chains_for_scores) {
    for(const auto& pair : chains_for_scores) {
        std::cout << "score: " << pair.first << std::endl;
        for(auto& chain : pair.second)
            print_chain(chain);
    }
}

void print_for_clang(const std::map<int, std::set<std::vector<std::size_t>>>& chains_for_scores) {
    std::cout << "#include \"anvil_chains.hpp\"\n"
              << "\n"
              << "const std::vector<std::vector<std::vector<std::uint8_t>>> g_anvil_chains{";

    for(const auto& pair : chains_for_scores) {
        std::cout << "{";
        for(auto& chain : pair.second) {
            std::cout << "{";
            for(std::size_t idx : chain)
                std::cout << static_cast<int>(g_techniques.at(idx).m_type) << ",";
            std::cout << "},";
        }
        std::cout << "},";
    }

    std::cout << "};\n";
}

int main() {
    std::map<int, std::set<std::vector<std::size_t>>> chains_for_scores;

    auto search_and_append = [&chains_for_scores](int begin, int end) {
        auto chains = filter_minimal_changed(find(begin, end));

        int score = end - begin;
        chains_for_scores[score].insert(chains.begin(), chains.end());
    };

    for(int i = 0; i <= g_max_score; ++i) {
        search_and_append(0, i);
        search_and_append(i, 0);
    }

    int max_score = std::accumulate(g_techniques.begin(), g_techniques.end(), 0, [](int max_score, const Technique& technique) {
        return std::max(max_score, std::abs(technique.m_score));
    });

    // Было жарко и было трудно думать. Поэтому я перестраховался и умножил на 2.
    int edge_score = max_score * 2;

    for(int begin = 0; begin <= edge_score; ++begin)
        for(int end = 0; end <= edge_score; ++end) {
            search_and_append(begin, end);
            search_and_append(end, begin);
        }

    for(int begin = g_max_score - edge_score; begin <= g_max_score; ++begin)
        for(int end = g_max_score - edge_score; end <= g_max_score; ++end) {
            search_and_append(begin, end);
            search_and_append(end, begin);
        }

    // print_for_human(chains_for_scores);
    print_for_clang(chains_for_scores);

    return 0;
}
