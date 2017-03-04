#pragma once

#include "Technique.hpp"

#include <array>
#include <vector>

class AnvilChainsGetter final {
    // считает количество смен техник
    // aabccd -- 4 смены
    static std::size_t countChanges(const std::vector<std::uint8_t>& chain);

    // Возвращает все подходящие под кузнечный паттерн последние 3 техники
    static std::vector<std::array<TechniqueType, 3>> getTechniques(const std::array<std::size_t, 3>& pattern);

    // Возвращает самые короткие цепочки
    static std::vector<std::vector<std::uint8_t>> getShorts(const std::vector<std::vector<std::uint8_t>>& chains);

    // Возвращает наименее меняющиеся цепочки
    static std::vector<std::vector<std::uint8_t>> getLessChanges(const std::vector<std::vector<std::uint8_t>>& chains);
public:
    static std::vector<std::uint8_t> get(const std::array<std::size_t, 3>& pattern, unsigned char score);
};
