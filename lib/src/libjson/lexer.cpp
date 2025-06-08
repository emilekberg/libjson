#include "lexer.h"
#include "libjson/token_types.h"
#include <algorithm>

namespace libjson {
Lexer::Lexer(const std::string_view &input) : _input(input), _exit(false) {
  _position = 0;
  _nextPosition = 1;
  _char = _input[0];
}

void Lexer::nextChar() {
  _position = _nextPosition;
  _nextPosition++;
  if (_position >= _input.size()) {
    _char = '\0';
    return;
  }
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
Token Lexer::next() {
  while (isWhitespace()) {
    nextChar();
  }
  if (isEndOfFile() || current() == '\0') {
    return {TokenTypes::END_OF_FILE, "\0"};
  }
  if (isSeparator()) {
    size_t start = _position;
    size_t end = _nextPosition;
    nextChar();
    return {TokenTypes::SEPARATOR, _input.substr(start, end - start)};
  }

  else if (isString()) {
    // strings can be either ' or ", so we store it and search for it.
    char search = current();
    nextChar();
    size_t start = _position;

    // search for the closing quote matching the entry one.
    // if it's escaped, ignore it.
    while (!(current() == search)) {
      // if we stumble opon an escape sign, we do some skips.
      if (current() == '\\') {
        // since this is escape sign, we skip it.
        nextChar();
        // this is the escaped sign, we skip this as well.
        nextChar();
      } else {
        // otherwise we just skip one
        nextChar();
      }
    }
    size_t end = _position;
    nextChar();
    return {TokenTypes::STRING, _input.substr(start, end - start)};
  }

  else if (isNumber()) {
    size_t start = _position;
    int numberOfDots = 0;
    // can only contain one minus and it has to be leading.
    if (current() == '-') {
      nextChar();
    }
    while (isDigit() || (current() == '.')) {
      if (current() == '.') {
        numberOfDots++;
      }
      nextChar();
    }
    size_t end = _position;
    std::string_view sub = _input.substr(start, end - start);
    if (numberOfDots > 1) {
      return {TokenTypes::ILLEGAL, sub};
    }
    return {TokenTypes::NUMBER, sub};
  }

  else if (isLiteral()) {
    std::string expected_literal = "";
    size_t start = _position;
    if (current() == 'f') {
      expected_literal = "false";
    } else if (current() == 't') {
      expected_literal = "true";
    } else if (current() == 'n') {
      expected_literal = "null";
    }
    size_t end = _position;
    for (char c : expected_literal) {
      if (current() != c) {
        end = _position;
        return {TokenTypes::ILLEGAL, _input.substr(start, end - start)};
      }
      nextChar();
    }
    end = start + expected_literal.size();
    return {TokenTypes::LITERAL, _input.substr(start, end - start)};
  }

  size_t start = std::max(0, (int)_position - 10);
  size_t count = std::min(20, (int)(_input.size() - start));
  return {TokenTypes::ILLEGAL, _input.substr(start, count)};
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
// TODO: this is not corrent
// should be able to use only the size to figure this out, not the current...
bool Lexer::isEndOfFile() {
  return _position >= _input.size() || current() == '\0';
}
bool Lexer::isNumber() { return isDigit() || _char == '-'; }
bool Lexer::isString() { return _char == '"' || _char == '\''; }
bool Lexer::isLiteral() { return _char == 'f' || _char == 't' || _char == 'n'; }
bool Lexer::isSeparator() {
  return _char == '{' || _char == '}' || _char == '[' || _char == ']' ||
         _char == ':' || _char == ',';
}
bool Lexer::isEscaped() { return peekPrev() == '\\'; }
bool Lexer::isEscaped(size_t offset) {
  return _input[_position + offset] == '\\';
}
bool Lexer::isDigit() { return (_char >= '0' && _char <= '9') || _char == '.'; }

} // namespace libjson
