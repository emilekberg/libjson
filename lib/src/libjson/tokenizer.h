#pragma once
#include "libjson/lexer.h"
#include "token.h"
#include <optional>
#include <vector>

namespace libjson {

class VectorTokenizer {
public:
  VectorTokenizer(std::string_view json);
  const Token &next();
  const Token &peek();

private:
  std::vector<Token> _tokens;
  size_t _pos{};
};

class LazyTokenizer {
public:
  LazyTokenizer(std::string_view json);
  Token next();
  const Token &peek();

private:
  std::optional<Token> _next;
  Lexer _lexer;
};

} // namespace libjson
