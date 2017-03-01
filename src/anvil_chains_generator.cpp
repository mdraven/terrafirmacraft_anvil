
#include <iostream>
#include <map>
#include <set>
#include <vector>

// максимальная глубина поиска
const std::size_t g_max_depth = 15;

// максимальное число очков; при переполнении этого значения заготовка исчезает
const std::size_t g_max_score = 150;

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
    std::string m_name;
};

const std::vector<Technique> g_techniques{
    {TechniqueType::hit_light, -3, "r1"},
    {TechniqueType::hit_medium, -6, "r2"},
    {TechniqueType::hit_heavy, -9, "r3"},
    {TechniqueType::draw, -15, "r4"},

    {TechniqueType::punch, 2, "g1"},
    {TechniqueType::bend, 7, "g2"},
    {TechniqueType::upset, 13, "g3"},
    {TechniqueType::shrink, 16, "g4"},
};

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

// Суммирует очки цепочки
// Если недопустимая комбинация, то возвращает -1
int calc_score(const std::vector<std::size_t>& chain) {
    int score = 0;

    for(std::size_t idx : chain) {
        score += g_techniques.at(idx).m_score;
        // при работе с наковальней очки не могут стать отрицательными или
        // больше максимума(это ломает предмет)
        if(score < 0 || score > g_max_score)
            return -1;
    }

    return score;
}

std::vector<std::vector<std::size_t>> find(unsigned int score) {
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

void print_for_human(const std::map<std::size_t, std::vector<std::vector<std::size_t>>>& chains_for_scores) {
    for(const auto& pair : chains_for_scores) {
        std::cout << "score: " << pair.first << std::endl;
        for(auto& chain : pair.second)
            print_chain(chain);
    }
}

void print_for_clang(const std::map<std::size_t, std::vector<std::vector<std::size_t>>>& chains_for_scores) {
    std::cout << "#include <cstdint>\n"
              << "#include <vector>\n"
              << "\n"
              << "const std::vector<std::vector<std::vector<std::uint8_t>>> anvil_chains{";

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

int main(int argc, char *argv[]) {
    std::map<std::size_t, std::vector<std::vector<std::size_t>>> chains_for_scores;

    for(std::size_t score = 0; score <= g_max_score; ++score) {
        auto chains = filter_minimal_changed(find(score));
        chains_for_scores[score] = chains;
    }

    // print_for_human(chains_for_scores);
    print_for_clang(chains_for_scores);

    return 0;
}
