#pragma once
#include "token.h"
#include <string>
#include <string_view>

namespace libjson {
class Lexer {
public:
  Lexer(const std::string_view &input);
  Token next();

  void nextChar();
  char current();
  char peekNext();
  char peekPrev();

  bool isWhitespace();
  bool isSeparator();
  bool isString();
  bool isNumber();
  bool isLiteral();
  bool isEscaped();
  bool isEscaped(size_t offset);
  bool isDigit();
  bool isEndOfFile();

private:
  std::string_view _input;
  size_t _position;
  size_t _nextPosition;
  char _char;
  bool _exit = false;
};

} // namespace libjson
