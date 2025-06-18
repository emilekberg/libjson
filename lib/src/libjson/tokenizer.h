#pragma once
#include "libjson/lexer.h"
#include "token.h"
#include <optional>
#include <vector>

namespace libjson {

class VectorTokenizer {
public:
  VectorTokenizer(std::istream &stream);
  const Token &next();
  const Token &peek();

private:
  std::vector<Token> _tokens;
  size_t _pos{};
};

class LazyTokenizer {
public:
  LazyTokenizer(std::istream &stream);
  Token next();
  const Token &peek();

private:
  std::optional<Token> _next;
  Lexer _lexer;
};

} // namespace libjson
