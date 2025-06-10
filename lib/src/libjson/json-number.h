#pragma once
#include "concepts.h"
#include <charconv>
#include <stdexcept>
#include <string>

namespace libjson {
class JSONNumber {
public:
  JSONNumber(const std::string &literal) : _literal(literal) {}

  template <concepts::Numeric T> T get() const {
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
