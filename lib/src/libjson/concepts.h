#pragma once

#include <type_traits>
namespace libjson::concepts {

template <typename T>
concept Numeric = std::is_arithmetic_v<T> && (!std::is_same_v<T, bool>);

template <typename T>
concept NonNumeric = !Numeric<T>;

template <typename T>
concept Int = std::is_integral_v<T>;

template <typename T>
concept Float = std::is_floating_point_v<T>;

} // namespace libjson::concepts
