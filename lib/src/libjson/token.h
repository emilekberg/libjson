#pragma once
#include "token_types.h"
#include <string>

namespace libjson {
struct Token {
  TokenTypes type;
  std::string literal;
};

} // namespace libjson
