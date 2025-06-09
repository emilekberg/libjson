#include "tokenizer.h"
#include <format>

namespace libjson {

VectorTokenizer::VectorTokenizer(std::string_view json) {
  for (const auto &token : Lexer(json)) {

    if (token.type == TokenTypes::ILLEGAL) {
      throw std::invalid_argument(
          std::format("ILLEGAL TOKEN FOUND: {}", token.literal));
    }
    _tokens.push_back(token);
  }
}
const Token &VectorTokenizer::next() { return _tokens[_pos++]; }
const Token &VectorTokenizer::peek() { return _tokens[_pos]; }

LazyTokenizer::LazyTokenizer(std::string_view json) : _lexer(json) {}
Token LazyTokenizer::next() {
  if (_next.has_value()) {
    Token t = std::move(_next.value());
    _next.reset();
    return t;
  }
  return _lexer.next();
}
const Token &LazyTokenizer::peek() {
  if (!_next.has_value()) {
    _next = _lexer.next();
  }
  return _next.value();
}
} // namespace libjson
