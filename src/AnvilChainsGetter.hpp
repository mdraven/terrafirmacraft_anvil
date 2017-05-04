#pragma once

#include "Technique.hpp"

#include <array>
#include <vector>

class AnvilChainsGetter final {
    // считает количество смен техник
    // aabccd -- 4 смены
    static std::size_t countChanges(const std::vector<std::uint8_t>& chain);

    // Возвращает все подходящие под кузнечный паттерн последние [0; 3] техники
    static std::vector<std::vector<TechniqueType>> getTechniques(const std::array<std::size_t, 3>& pattern);

    // Возвращает самые короткие цепочки
    static std::vector<std::vector<std::uint8_t>> getShorts(const std::vector<std::vector<std::uint8_t>>& chains);

    // Возвращает наименее меняющиеся цепочки
    static std::vector<std::vector<std::uint8_t>> getLessChanges(const std::vector<std::vector<std::uint8_t>>& chains);

    // Возвращает цепочки которые не выходят за границы шкалы
    static std::vector<std::vector<std::uint8_t>> getIntoScaleLimits(int begin_score, const std::vector<std::vector<std::uint8_t>>& chains);

    // Проверить на вхождение в границы шкалы
    static bool checkScaleLimits(int score);

    // Возвращает score которое будет до применения техник techniques, или -1 в случае выхода за границы шкалы
    static int getScoreBefore(int end_score, const std::vector<TechniqueType>& techniques);
public:
    static std::vector<std::uint8_t> get(const std::array<std::size_t, 3>& pattern, int begin_score, int end_score);
};
