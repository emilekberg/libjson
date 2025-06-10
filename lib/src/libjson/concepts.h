#pragma once

#include <type_traits>
namespace libjson::concepts {

template <typename T>
concept Numeric = std::is_arithmetic_v<T> && (!std::is_same_v<T, bool>);

template <typename T>
concept NonNumeric = !Numeric<T>;

} // namespace libjson::concepts
