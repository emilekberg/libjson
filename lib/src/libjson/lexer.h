#pragma once
#include "token.h"
#include <string>

namespace libjson {
class Lexer {
public:
  Lexer(std::string input);
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
  bool isDigit();
  bool isEndOfFile();

private:
  std::string _input;
  size_t _position;
  size_t _nextPosition;
  char _char;
  bool _exit = false;
};

} // namespace libjson
