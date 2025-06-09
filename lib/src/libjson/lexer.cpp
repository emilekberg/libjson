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
char Lexer::current() const { return _char; }

Token Lexer::next() {
  while (isWhitespace()) {
    nextChar();
  }
  // used when iterating to also return the EOF token.
  if (_position > _input.size()) {
    return {TokenTypes::NONE, "\0"};
  }
  if (isEndOfFile()) {
    ++_position;
    return {TokenTypes::END_OF_FILE, "\0"};
  }
  if (isSeparator()) {
    size_t start = _position;
    size_t end = _nextPosition;
    nextChar();
    return {TokenTypes::SEPARATOR, _input.substr(start, end - start)};
  }

  else if (isString()) {
    nextChar();
    size_t start = _position;

    // search for the closing quote matching the entry one.
    // if it's escaped, ignore it.
    while (!isString()) {
      // if we stumble opon an escape sign, we do some skips.
      // TODO: validate what we can skip. only certain characters can be
      // escaped according to the specification.
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
    std::string error = "";
    // can only contain one minus and it has to be leading.
    if (current() == '-') {
      nextChar();
    }

    // 0 can only be first, or fraction.
    if (current() == '0') {
      nextChar();
      // if we encounter a 0, then a digit, it's invalid.
      if (isDigit()) {
        error = "leading zero";
      }
    } else {
      // if we didn't encounter a zero, proceed looking at the following numbers
      while (isDigit()) {
        nextChar();
      }
    }

    // if we encounter a dot, process all numbers succeeding it.
    if (current() == '.') {
      nextChar();
      while (isDigit()) {
        nextChar();
      }
    }

    // if the number if exponent, we need to treat that.
    if (isNumberExponentComponent()) {
      nextChar();
      if ((current() == '+' || current() == '-')) {
        nextChar();
      }

      // validate that we have at least 1 digit after the exponent.
      if (!isDigit()) {
        error = "missing digit after exponent";
      }
      while (isDigit()) {
        nextChar();
      }
    }

    size_t end = _position;
    std::string_view sub = _input.substr(start, end - start);
    // if we have a . here we have an issue mr president.
    if (current() == '.') {
      error = "too many .";
    }
    if (!error.empty()) {
      return {TokenTypes::ILLEGAL, error};
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
    for (const char &c : expected_literal) {
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

bool Lexer::isWhitespace() const {
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
bool Lexer::isEndOfFile() const { return _position >= _input.size(); }
bool Lexer::isNumber() const { return isDigit() || _char == '-'; }
bool Lexer::isNumberExponentComponent() const {
  return _char == 'e' || _char == 'E';
}
bool Lexer::isString() const { return _char == '"'; }
bool Lexer::isLiteral() const {
  return _char == 'f' || _char == 't' || _char == 'n';
}
bool Lexer::isSeparator() const {
  return _char == '{' || _char == '}' || _char == '[' || _char == ']' ||
         _char == ':' || _char == ',';
}
bool Lexer::isDigit() const { return (_char >= '0' && _char <= '9'); }

} // namespace libjson
