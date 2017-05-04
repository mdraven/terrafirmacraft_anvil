
#include "increment.hpp"

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
