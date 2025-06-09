#pragma once
#include <charconv>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace libjson {
class JSONNumber {
public:
  JSONNumber(const std::string &literal) : _literal(literal) {}

  template <typename T> const T get() const {
    static_assert(std::is_arithmetic_v<T>, "T must be an arithmetic type");
    T value;
    auto result = std::from_chars(_literal.data(),
                                  _literal.data() + _literal.size(), value);
    if (result.ec != std::errc{}) {
      throw std::invalid_argument("Invalid integer: " + _literal);
    }
    return value;
  }

private:
  std::string _literal;
};

} // namespace libjson
