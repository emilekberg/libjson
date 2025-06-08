#pragma once
#include <charconv>
#include <stdexcept>
#include <string>

namespace libjson {
class JSONNumber {
public:
  JSONNumber(const std::string &literal) : _literal(literal) {}

  template <typename T> T get() {
    T value;
    const char *begin = _literal.data();
    const char *end = _literal.data() + _literal.size();

    auto result = std::from_chars(begin, end, value);
    if (result.ec != std::errc{}) {
      throw std::invalid_argument("Invalid integer: " + _literal);
    }
    return value;
  }

private:
  std::string _literal;
};

} // namespace libjson
