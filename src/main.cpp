
#include "Config.hpp"
#include "FindTechniques.hpp"
#include "Technique.hpp"

#include <iostream>
#include <stdexcept>

void print_chain(const std::vector<Technique>& techniques, const std::vector<std::size_t>& chain) {
    for(std::size_t idx : chain)
        std::cout << get_name(techniques.at(idx).m_type) << " ";

    std::cout << std::endl;
}

int main(int argc, char *argv[]) {
    try {
        const std::vector<Technique> techniques{
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

        Config config;
        FindTechniques ft(techniques, config);
        auto results = ft.find({any_hammer, any_hammer, g3}, 238/2);

        for(auto& result : results)
            print_chain(techniques, result);
    }
    catch(const std::exception& e) {
        std::cout << "Exception was catched in main(): " << e.what() << std::endl;
        throw;
    }

    return 0;
}
