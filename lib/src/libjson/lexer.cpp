#include "lexer.h"
#include "libjson/token_types.h"
#include <memory_resource>

namespace libjson {
Lexer::Lexer(std::string input) : _input(input), _exit(false) {
  _position = 0;
  _nextPosition = 1;
  _char = _input[0];
}

Token Lexer::next() {
  while (isWhitespace()) {
    nextChar();
  }
  if (isSeparator()) {
    return {TokenTypes::SEPARATOR, &""[_char]};
  } else if (isString()) {
    char search = _char;
    nextChar();
    size_t start = _position;
    while (!(current() == search && !isEscaped())) {
      nextChar();
    }
    size_t end = _position;
    return {TokenTypes::STRING, _input.substr(start, end - start)};
  } else if (isNumber()) {
    size_t start = _position;
    int numberOfDots = 0;
    while (isNumber() || (current() == '.')) {
      if (current() == '.') {
        numberOfDots++;
      }
      nextChar();
    }
    size_t end = _position + 1;
    std::string sub = _input.substr(start, end - start);
    if (numberOfDots > 1) {
      return {TokenTypes::ILLEGAL, sub};
    }
    return {TokenTypes::NUMBER, sub};
  } else if (isLiteral()) {
  }

  return {TokenTypes::ILLEGAL, ""};
}

void Lexer::nextChar() {
  if (_nextPosition >= _input.size()) {
    _char = '\0';
    return;
  }
  _position = _nextPosition;
  _nextPosition++;
  _char = _input[_position];
}
char Lexer::current() { return _char; }
char Lexer::peekNext() { return _input[_nextPosition]; }
char Lexer::peekPrev() {
  if (_position == 0) {
    return false;
  }
  return _input[_position - 1];
}
bool Lexer::isWhitespace() {
  if (_char == ' ') {
    return true;
  }
  if (_char == '\t') {
    return true;
  }
  if (_char == '\n') {
    return true;
  }
  if (_char == '\r') {
    return true;
  }
  return false;
}
bool Lexer::isNumber() { return isDigit() || _char == '-'; }
bool Lexer::isString() { return _char == '"' || _char == '\''; }
bool Lexer::isLiteral() { return _char == 'f' || _char == 't' || _char == 'n'; }
bool Lexer::isSeparator() {
  return _char == '{' || _char == '}' || _char == '[' || _char == ']' ||
         _char == ':' || _char == ',';
}
bool Lexer::isEscaped() { return peekPrev() == '\\'; }
bool Lexer::isDigit() { return (_char >= '0' && _char <= '9') || _char == '.'; }

} // namespace libjson
