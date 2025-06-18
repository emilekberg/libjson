#include "lexer.h"
#include "libjson/token_types.h"
#include <ios>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

namespace libjson {
Lexer::Lexer(std::istream &ss) : _ss(ss), _position{} { nextChar(); }

void Lexer::nextChar() {
  // std::cout << _current << std::endl;

  _ss.get(_current);
}
const char Lexer::current() const { return _current; }

Token Lexer::next() {
  while (isWhitespace() && _ss.good()) {
    nextChar();
  }
  if (isEndOfFile()) {
    if (_end) {
      return {TokenTypes::NONE, "\0"};
    }
    _end = true;
    return {TokenTypes::END_OF_FILE, "\0"};
  }

  if (isSeparator()) {
    char c = _current;
    nextChar();
    return {TokenTypes::SEPARATOR, std::string(1, c)};
  }

  if (isString()) {
    std::ostringstream oss;

    // skip one
    nextChar();

    // search for the closing quote matching the entry one.
    // if it's escaped, ignore it.
    while (!isString()) {
      // if we stumble opon an escape sign, we do some skips.
      if (current() == '\\') {
        // since this is escape sign, we skip it.
        oss << _current;
        nextChar();

        // TODO: we are now at the escaped sign.
        // we need to verify what this is, or return invalid token.
      }

      oss << _current;
      nextChar();
    }
    nextChar();

    return {TokenTypes::STRING, oss.str()};
  }

  if (isNumber()) {
    std::ostringstream oss;
    std::string error;
    // can only contain one minus and it has to be leading.
    if (current() == '-') {
      oss << _current;
      nextChar();
    }

    // 0 can only be first, or fraction.
    if (current() == '0') {

      oss << _current;
      nextChar();
      // if we encounter a 0, then a digit, it's invalid.
      if (isDigit()) {
        error = "leading zero";
      }
    } else {
      // if we didn't encounter a zero, proceed looking at the following numbers
      while (isDigit()) {

        oss << _current;
        nextChar();
      }
    }

    // if we encounter a dot, process all numbers succeeding it.
    if (current() == '.') {

      oss << _current;
      nextChar();
      while (isDigit()) {
        oss << _current;
        nextChar();
      }
    }

    // if the number if exponent, we need to treat that.
    if (isNumberExponentComponent()) {

      oss << _current;
      nextChar();
      if ((current() == '+' || current() == '-')) {

        oss << _current;
        nextChar();
      }

      // validate that we have at least 1 digit after the exponent.
      if (!isDigit()) {
        error = "missing digit after exponent";
      }
      while (isDigit()) {

        oss << _current;
        nextChar();
      }
    }

    if (current() == '.') {
      error = "too many .";
    }
    if (!error.empty()) {
      return {TokenTypes::ILLEGAL, oss.str()};
    }
    // nextChar();
    return {TokenTypes::NUMBER, oss.str()};
  }

  if (isLiteral()) {
    std::string_view expected_literal;
    if (current() == 'f') {
      expected_literal = "false";
    } else if (current() == 't') {
      expected_literal = "true";
    } else if (current() == 'n') {
      expected_literal = "null";
    }
    for (const char &c : expected_literal) {
      if (current() != c) {
        return {TokenTypes::ILLEGAL, ""};
      }
      nextChar();
    }
    return {TokenTypes::LITERAL, std::string(expected_literal)};
  }

  return {TokenTypes::ILLEGAL, ""};
}

bool Lexer::isWhitespace() const {
  return current() == ' ' || current() == '\t' || current() == '\n' ||
         current() == '\r';
}
bool Lexer::isEndOfFile() const { return _ss.eof(); }
bool Lexer::isNumber() const { return isDigit() || current() == '-'; }
bool Lexer::isNumberExponentComponent() const {
  return current() == 'e' || current() == 'E';
}
bool Lexer::isString() const { return current() == '"'; }
bool Lexer::isLiteral() const {
  return current() == 'f' || current() == 't' || current() == 'n';
}
bool Lexer::isSeparator() const {
  return current() == '{' || current() == '}' || current() == '[' ||
         current() == ']' || current() == ':' || current() == ',';
}
bool Lexer::isDigit() const { return (current() >= '0' && current() <= '9'); }

} // namespace libjson
