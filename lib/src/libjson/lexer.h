#pragma once
#include "token.h"
namespace libjson {
class Lexer {
public:
  Lexer(std::istream &ss);
  Lexer(std::istream &&ss);
  Token next();

  struct Iterator {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;

    Iterator(Lexer &lexer, const Token &token)
        : _lexer(lexer), _current(token) {}

    const Token &operator*() const { return _current; }
    const Token *operator->() const { return &_current; }

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
    Token _current;
  };

  Iterator begin() { return Iterator(*this, this->next()); }
  Iterator end() { return Iterator(*this, Token_End); }

private:
  inline void nextChar();
  inline const char current() const;
  inline void fillbuffer();

  inline bool isWhitespace() const;
  inline bool isSeparator() const;
  inline bool isString() const;
  inline bool isNumber() const;
  inline bool isLiteral() const;
  inline bool isDigit() const;
  inline bool isNumberExponentComponent() const;
  inline bool isEndOfFile() const;

  std::istream &_ss;
  char _buffer[512];
  size_t _buffer_position;
  size_t _buffer_length;
  bool _end = false;
  char _current;
};
} // namespace libjson
