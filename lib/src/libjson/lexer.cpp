#include "lexer.h"
#include "libjson/exceptions.h"
#include "libjson/token-types.h"
#include <iostream>
#include <string>
#include <string_view>

namespace libjson {
Lexer::Lexer(std::istream &stream) : _stream(stream), _buffer{} {
   fillbuffer();
}
void Lexer::fillbuffer() {
   _stream.read(_buffer.data(), sizeof(_buffer));
   _bufferSize     = _stream.gcount();
   _bufferPosition = 0;
}
void Lexer::nextChar() {
   _bufferPosition++;
   if (_bufferPosition >= _bufferSize) [[unlikely]] {
      fillbuffer();
   }
   // std::cout << current() << std::endl;
}
const char Lexer::current() const {
   if (_bufferPosition >= _bufferSize) {
      return '\0';
   }
   return _buffer[_bufferPosition];
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

   switch (current()) {
   //== SEPARATORS ==//
   case '{':
      nextChar();
      return {TokenTypes::CURLY_BRACE_OPEN};
   case '}':
      nextChar();
      return {TokenTypes::CURLY_BRACE_CLOSE};
   case '[':
      nextChar();
      return {TokenTypes::SQUARE_BRACKET_OPEN};
   case ']':
      nextChar();
      return {TokenTypes::SQUARE_BRACKET_CLOSE};
   case ',':
      nextChar();
      return {TokenTypes::COMMA};
   case ':':
      nextChar();
      return {TokenTypes::COLON};

   //== LITERAL ==//
   case 'f':
   case 't':
   case 'n':
      return lexLiteral();

   //== STRING ==//
   case '"':
      return lexString();

   //== NUMBERS ==//
   case '-':
      return lexNumber(NumberState::Negative);
   case '0':
      return lexNumber(NumberState::Zero);
   case '1':
   case '2':
   case '3':
   case '4':
   case '5':
   case '6':
   case '7':
   case '8':
   case '9':
      return lexNumber(NumberState::Integer);
   default:
      throw unexpected_token("Lexer: unknown token");
   }
}

Token Lexer::lexNumber(NumberState state) {
   std::string result;
   while (state != NumberState::Done) {
      switch (state) {
      case NumberState::Negative: {
         result.push_back(current());
         nextChar();
         if (_buffer[_bufferPosition] == '0')
            state = NumberState::Zero;
         else
            state = NumberState::Integer;
      } break;

      case NumberState::Zero: {
         result.push_back(current());
         nextChar();
         if (current() == '.') {
            state = NumberState::Dot;
         } else if (isDigit()) {
            throw unexpected_token("cannot have leading zeroes");
         } else {
            state = NumberState::Done;
         }
      } break;

      case NumberState::Integer: {
         while (isDigit()) {
            appendSpan(result, [](char c) { return c >= '0' && c <= '9'; });
         }
         if (current() == '.') {
            state = NumberState::Dot;
         } else if (isNumberExponentComponent()) {
            state = NumberState::ExpStart;
         } else {
            state = NumberState::Done;
         }
      } break;

      case NumberState::Dot: {
         result.push_back(current());
         nextChar();
         if (isDigit()) {
            state = NumberState::Fraction;
         } else {
            throw unexpected_token("expected digit, but got some other shit");
         }
      } break;

      case NumberState::Fraction: {
         while (isDigit()) {
            appendSpan(result, [](char c) { return c >= '0' && c <= '9'; });
         }
         if (isNumberExponentComponent()) {
            state = NumberState::ExpStart;
         } else if (current() == '.') {
            throw unexpected_token("Lexer: Number cannot have multiple dots");
         } else {
            state = NumberState::Done;
         }
      } break;

      case NumberState::ExpStart: {
         result.push_back(current());
         nextChar();
         if (current() == '-' || current() == '+') {
            state = NumberState::ExpSign;
         } else if (isDigit()) {
            state = NumberState::Exp;
         } else {
            throw unexpected_token("expected digit, but got some other shit");
         }
      } break;

      case NumberState::ExpSign: {
         result.push_back(current());
         nextChar();
         if (isDigit()) {
            state = NumberState::Exp;
         } else {
            throw unexpected_token("expected digit, but got some other shit");
         }
      } break;

      case NumberState::Exp: {
         while (isDigit()) {
            appendSpan(result, [](char c) { return c >= '0' && c <= '9'; });
         }
         state = NumberState::Done;
      } break;

      case NumberState::Done:
         break;
      }
   }

   return {TokenTypes::NUMBER, std::move(result)};
}

Token Lexer::lexString() {
   std::string result;

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
      }
      appendSpan(result, [](char c) { return c != '\\' && c != '"'; });
   }
   // skip the closing quote
   nextChar();
   // std::cout << result << std::endl;
   return {TokenTypes::STRING, std::move(result)};
}
Token Lexer::lexLiteral() {
   std::string_view expected_literal;
   switch (current()) {
   case 'f': {
      expected_literal = "false";
   } break;
   case 't': {
      expected_literal = "true";
   } break;
   case 'n': {
      expected_literal = "null";
   } break;
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
   return whitespaceLUT[current()];
}

bool Lexer::isEndOfFile() const {
   return _bufferPosition >= _bufferSize && _stream.eof();
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
