#pragma once

#include <cstdint>
#include <vector>

// 0, 1, 00, 01, 10, 11, 000, ...
// Возвращает все комбинации от самой короткой к самой длинной. Реализует обход
// дерева в ширину.
void increment(std::vector<std::size_t>& vec, std::size_t mod);
