#include "lexer.h"
#include "libjson/token_types.h"
#include <string>

namespace libjson {
Lexer::Lexer(const std::string_view &input) : _input(input), _position{} {}
Lexer::Lexer(const std::string_view &&input)
    : _input(std::move(input)), _position{} {}

void Lexer::nextChar() { ++_position; }
const char &Lexer::current() const { return _input[_position]; }

Token Lexer::next() {
  while (isWhitespace()) {
    nextChar();
  }
  if (isEndOfFile()) {
    // if we have read beyond input size, have read the entire buffer.
    // this is used when using the iterator.
    if (_position > _input.size()) {
      return {TokenTypes::NONE, "\0"};
    }
    // increment the position beyond the input, this signals that we have
    // processed the entire buffer.
    ++_position;
    return {TokenTypes::END_OF_FILE, "\0"};
  }
  if (isSeparator()) {
    nextChar();
    return {TokenTypes::SEPARATOR, _input.substr(_position - 1, 1)};
  }

  if (isString()) {
    nextChar();
    size_t start = _position;

    // search for the closing quote matching the entry one.
    // if it's escaped, ignore it.
    while (!isString()) {
      // if we stumble opon an escape sign, we do some skips.
      if (current() == '\\') {
        // since this is escape sign, we skip it.
        nextChar();
        // TODO: we are now at the escaped sign.
        // we need to verify what this is, or return invalid token.
      }
      nextChar();
    }
    size_t end = _position;
    nextChar();
    return {TokenTypes::STRING, _input.substr(start, end - start)};
  }

  if (isNumber()) {
    size_t start = _position;
    std::string error;
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
    if (current() == '.') {
      error = "too many .";
    }
    if (!error.empty()) {
      return {TokenTypes::ILLEGAL, sub};
    }
    return {TokenTypes::NUMBER, sub};
  }

  if (isLiteral()) {
    std::string_view expected_literal;
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
    return {TokenTypes::LITERAL, _input.substr(start, expected_literal.size())};
  }

  return {TokenTypes::ILLEGAL, ""};
}

bool Lexer::isWhitespace() const {
  return current() == ' ' || current() == '\t' || current() == '\n' ||
         current() == '\r';
}
bool Lexer::isEndOfFile() const { return _position >= _input.size(); }
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
