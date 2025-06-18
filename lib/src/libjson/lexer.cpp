#include "lexer.h"
#include "libjson/token_types.h"
#include <iostream>
#include <string>
#include <string_view>

namespace libjson {
Lexer::Lexer(std::istream &stream) : _stream(stream), _buffer{} {
  // todo
  fillbuffer();
}
void Lexer::fillbuffer() {
  _stream.read(_buffer, sizeof(_buffer));
  _buffer_length = _stream.gcount();
  _buffer_position = 0;
}
void Lexer::nextChar() {
  _buffer_position++;
  if (_buffer_position >= _buffer_length) {
    fillbuffer();
  }
}
const char Lexer::current() const {
  if (_buffer_position >= _buffer_length) {
    return '\0';
  }
  return _buffer[_buffer_position];
}

Token Lexer::next() {
  while (isWhitespace()) {
    nextChar();
  }
  if (isEndOfFile()) {
    if (_end) {
      return {TokenTypes::END, "\0"};
    }
    _end = true;
    return {TokenTypes::END_OF_FILE, "\0"};
  }

  if (isSeparator()) {
    char c = current();
    nextChar();
    return {TokenTypes::SEPARATOR, std::string(1, c)};
  }

  if (isString()) {
    std::string buffer;
    buffer.reserve(64);

    // skip one
    nextChar();

    // search for the closing quote matching the entry one.
    // if it's escaped, ignore it.
    while (!isString()) {
      // if we stumble opon an escape sign, we do some skips.
      if (current() == '\\') {
        // since this is escape sign, we skip it.
        buffer.push_back(current());
        nextChar();

        // TODO: we are now at the escaped sign.
        // we need to verify what this is, or return invalid token.
      }

      buffer.push_back(current());
      nextChar();
    }
    nextChar();

    return {TokenTypes::STRING, std::move(buffer)};
  }

  if (isNumber()) {
    std::string buffer;
    buffer.reserve(64);
    std::string error;
    // can only contain one minus and it has to be leading.
    if (current() == '-') {
      buffer.push_back(current());
      nextChar();
    }

    // 0 can only be first, or fraction.
    if (current() == '0') {

      buffer.push_back(current());
      nextChar();
      // if we encounter a 0, then a digit, it's invalid.
      if (isDigit()) {
        error = "leading zero";
      }
    } else {
      // if we didn't encounter a zero, proceed looking at the following numbers
      while (isDigit()) {

        buffer.push_back(current());
        nextChar();
      }
    }

    // if we encounter a dot, process all numbers succeeding it.
    if (current() == '.') {

      buffer.push_back(current());
      nextChar();
      while (isDigit()) {
        buffer.push_back(current());
        nextChar();
      }
    }

    // if the number if exponent, we need to treat that.
    if (isNumberExponentComponent()) {

      buffer.push_back(current());
      nextChar();
      if ((current() == '+' || current() == '-')) {

        buffer.push_back(current());
        nextChar();
      }

      // validate that we have at least 1 digit after the exponent.
      if (!isDigit()) {
        error = "missing digit after exponent";
      }
      while (isDigit()) {

        buffer.push_back(current());
        nextChar();
      }
    }

    if (current() == '.') {
      error = "too many .";
    }
    if (!error.empty()) {
      return {TokenTypes::ILLEGAL, std::move(buffer)};
    }
    // nextChar();
    return {TokenTypes::NUMBER, std::move(buffer)};
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
  char c = current();
  return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

bool Lexer::isEndOfFile() const {
  return _buffer_position >= _buffer_length && _stream.eof();
}

bool Lexer::isNumber() const { return isDigit() || current() == '-'; }

bool Lexer::isNumberExponentComponent() const {
  return current() == 'e' || current() == 'E';
}

bool Lexer::isString() const { return current() == '"'; }

bool Lexer::isLiteral() const {
  return current() == 't' || current() == 'f' || current() == 'n';
}

bool Lexer::isSeparator() const {
  char c = current();
  return c == '{' || c == '}' || c == '[' || c == ']' || c == ':' || c == ',';
}

bool Lexer::isDigit() const {
  char c = current();
  return c >= '0' && c <= '9';
}

} // namespace libjson
