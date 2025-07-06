#pragma once
#include "token.h"
#include <array>
#include <optional>
namespace libjson {
class Lexer {
 public:
   Lexer(std::istream &stream);
   Lexer(std::istream &&stream);
   Token next();
   Token peek();

   struct Iterator {
      using iterator_category = std::forward_iterator_tag;
      using difference_type   = std::ptrdiff_t;

      Iterator(Lexer &lexer, const Token &token) : _lexer(lexer), _current(token) {}

      const Token &operator*() const {
         return _current;
      }
      const Token *operator->() const {
         return &_current;
      }

      Iterator &operator++() {
         _current = std::move(_lexer.next());
         return *this;
      }

      friend bool operator==(const Iterator &lhs, const Iterator &rhs) {
         return lhs._current.type == rhs._current.type;
      };

      friend bool operator!=(const Iterator &lhs, const Iterator &rhs) {
         return lhs._current.type != rhs._current.type;
      };

    private:
      Lexer &_lexer;
      Token  _current;
   };

   Iterator begin() {
      return Iterator(*this, this->next());
   }
   Iterator end() {
      return Iterator(*this, Token_End);
   }

 private:
   Token                           tokenize();
   void                            nextChar();
   [[nodiscard]] inline const char current() const;
   void                            fillbuffer();

   [[nodiscard]] bool isWhitespace() const;
   [[nodiscard]] bool isSeparator() const;
   [[nodiscard]] bool isString() const;
   [[nodiscard]] bool isNumber() const;
   [[nodiscard]] bool isLiteral() const;
   [[nodiscard]] bool isDigit() const;
   [[nodiscard]] bool isNumberExponentComponent() const;
   [[nodiscard]] bool isAllowedEscapeChar() const;
   [[nodiscard]] bool isEndOfFile() const;
   [[nodiscard]] bool isHexDecimal() const;

   Token lexSeparator();
   Token lexNumber();
   Token lexString();
   Token lexLiteral();

   std::istream          &_stream;
   std::array<char, 4096> _buffer;
   size_t                 _bufferPosition;
   size_t                 _bufferSize;
   bool                   _end = false;
   std::optional<Token>   _next;
};
} // namespace libjson
