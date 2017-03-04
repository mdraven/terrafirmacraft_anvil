#pragma once

#include <type_traits>

template<typename T>
constexpr typename std::underlying_type<T>::type to_under(T t) {
    return static_cast<typename std::underlying_type<T>::type>(t);
}
