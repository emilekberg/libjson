#pragma once
#include "token.h"
#include <string_view>

namespace libjson {
class Lexer {
public:
  Lexer(const std::string_view &input);
  Token next();

private:
  inline void nextChar();
  inline char current() const;

  inline bool isWhitespace() const;
  inline bool isSeparator() const;
  inline bool isString() const;
  inline bool isNumber() const;
  inline bool isLiteral() const;
  inline bool isDigit() const;
  inline bool isNumberExponentComponent() const;
  inline bool isEndOfFile() const;

  std::string_view _input;
  size_t _position;
  size_t _nextPosition;
  char _char;
  bool _exit = false;
};

} // namespace libjson
