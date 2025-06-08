#pragma once
#include <charconv>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace libjson {
class JSONNumber {
public:
  JSONNumber(const std::string &literal) : _literal(literal) {}

  template <typename T> T get() {
    // if constexpr (std::is_floating_point_v<T>) {
    //   if constexpr (std::is_same_v<T, float>) {
    //     return std::stof(_literal);
    //   } else if constexpr (std::is_same_v<T, double>) {
    //     return std::stod(_literal);
    //   } else if constexpr (std::is_same_v<T, long double>) {
    //     return std::stold(_literal);
    //   }
    // } else
    // if constexpr (std::is_integral_v<T>) {
    T value;
    const char *begin = _literal.data();
    const char *end = _literal.data() + _literal.size();

    auto result = std::from_chars(begin, end, value);
    if (result.ec != std::errc{}) {
      throw std::invalid_argument("Invalid integer: " + _literal);
    }
    return value;
    // } else {
    //   static_assert(always_false<T>, "Unsupported type");
    // }
  }

private:
  std::string _literal;

  template <typename> static constexpr bool always_false = false;
};

} // namespace libjson
