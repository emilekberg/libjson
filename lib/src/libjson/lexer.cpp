#include "lexer.h"
#include "libjson/exceptions.h"
#include "libjson/token_types.h"
#include <iostream>
#include <string>
#include <string_view>

namespace libjson {
Lexer::Lexer(std::istream &stream) : _stream(stream), _buffer{} {
   fillbuffer();
}
void Lexer::fillbuffer() {
   _stream.read(_buffer.data(), sizeof(_buffer));
   _buffer_length   = _stream.gcount();
   _buffer_position = 0;
}
void Lexer::nextChar() {
   _buffer_position++;
   if (_buffer_position >= _buffer_length) {
      fillbuffer();
   }
   // std::cout << current() << std::endl;
}
const char Lexer::current() const {
   if (_buffer_position >= _buffer_length) {
      return '\0';
   }
   return _buffer[_buffer_position];
}

Token Lexer::peek() {
   if (!_next.has_value()) {
      _next = tokenize();
   }
   return _next.value();
}

Token Lexer::next() {
   if (_next.has_value()) {
      Token token = std::move(_next.value());
      _next.reset();
      return token;
   }
   return tokenize();
}

Token Lexer::tokenize() {
   while (isWhitespace()) [[likely]] {
      nextChar();
   }

   if (isEndOfFile()) [[unlikely]] {
      if (_end) {
         return {TokenTypes::END};
      }
      _end = true;
      return {TokenTypes::END_OF_FILE};
   }

   if (isSeparator()) [[likely]] {
      return lexSeparator();
   }

   if (isString()) [[likely]] {
      return lexString();
   }

   if (isNumber()) [[likely]] {
      return lexNumber();
   }

   if (isLiteral()) [[likely]] {
      return lexLiteral();
   }
   throw unexpected_token("Lexer: unknown token");
}
Token Lexer::lexSeparator() {

   char c = current();
   nextChar();
   switch (c) {
   case '{':
      return {TokenTypes::CURLY_BRACE_OPEN};
   case '}':
      return {TokenTypes::CURLY_BRACE_CLOSE};
   case '[':
      return {TokenTypes::SQUARE_BRACKET_OPEN};
   case ']':
      return {TokenTypes::SQUARE_BRACKET_CLOSE};
   case ',':
      return {TokenTypes::COMMA};
   case ':':
      return {TokenTypes::COLON};
   }
   throw unexpected_token("Lexer: unexpected separator.");
}

Token Lexer::lexNumber() {
   std::string buffer;
   buffer.reserve(16);
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
      if (isDigit()) [[unlikely]] {
         throw unexpected_token("Lexer: Unexpected 0, number cannot start with two 0's");
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
   if (isNumberExponentComponent()) [[unlikely]] {

      buffer.push_back(current());
      nextChar();
      if ((current() == '+' || current() == '-')) {

         buffer.push_back(current());
         nextChar();
      }

      // validate that we have at least 1 digit after the exponent.
      if (!isDigit()) [[unlikely]] {
         throw unexpected_token("Lexer: missing digit in exponent number");
      }
      while (isDigit()) {

         buffer.push_back(current());
         nextChar();
      }
   }

   if (current() == '.') [[unlikely]] {
      throw unexpected_token("Lexer: number cannot contain more than one .(dot)");
   }
   return {TokenTypes::NUMBER, std::move(buffer)};
}

Token Lexer::lexString() {
   std::string result;
   result.reserve(64);

   // skip the opening quote
   nextChar();

   while (!isString()) {
      if (current() == '\\') {
         result.push_back(current());
         nextChar();

         if (!isAllowedEscapeChar()) {
            throw unexpected_token(std::format("got unexpected escaped character: {}", current()));
         } else if (current() == 'u') {
            for (size_t i = 0; i < 4; i++) {
               result.push_back(current());
               nextChar();
               if (!isHexDecimal()) {
                  throw unexpected_token(std::format("got illegal hex number: {}", current()));
               }
            }
         } else {
            result.push_back(current());
            nextChar();
         }
      } else {
         size_t start = _buffer_position;
         while (_buffer_position < _buffer_length && _buffer[_buffer_position] != '"' &&
                _buffer[_buffer_position] != '\\') {
            _buffer_position++;
         }
         result.append(_buffer.data() + start, _buffer_position - start);
         // std::cout << result << std::endl;
         if (_buffer_position >= _buffer_length) {
            fillbuffer();
         }
      }

      // buffer.push_back(current());
      // nextChar();
   }
   // skip the closing quote
   nextChar();
   // std::cout << result << std::endl;
   return {TokenTypes::STRING, std::move(result)};
}
Token Lexer::lexLiteral() {
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
         throw unexpected_token("Lexer: expected literal(true,false,null) but got something else");
      }
      nextChar();
   }
   return {TokenTypes::LITERAL, std::string(expected_literal)};
}
bool Lexer::isWhitespace() const {
   char c = current();
   return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

bool Lexer::isEndOfFile() const {
   return _buffer_position >= _buffer_length && _stream.eof();
}

bool Lexer::isNumber() const {
   return isDigit() || current() == '-';
}

bool Lexer::isNumberExponentComponent() const {
   return current() == 'e' || current() == 'E';
}

bool Lexer::isString() const {
   return current() == '"';
}

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

bool Lexer::isAllowedEscapeChar() const {
   char c = current();
   return c == '"' || c == '\\' || c == '/' || c == 'b' || c == 'f' || c == 'n' || c == 'r' ||
          c == 't' || c == 'u';
}

bool Lexer::isHexDecimal() const {
   char c = current();
   return isDigit() || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

} // namespace libjson
