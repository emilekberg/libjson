#pragma once
#include "token.h"
#include <sstream>
namespace libjson {
class Lexer {
public:
  Lexer(std::istream &ss);
  Lexer(std::istream &&ss);
  Lexer(std::string_view str);
  Token next();

  struct Iterator {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;

    Iterator(Lexer &lexer, const Token &token)
        : _lexer(lexer), _current(token) {}

    const Token &operator*() const { return _current; }
    const Token *operator->() const { return &_current; }

    Iterator &operator++() {
      _current = _lexer.next();
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
  Iterator end() { return Iterator(*this, Token_None); }

private:
  inline void nextChar();
  inline const char current() const;

  inline bool isWhitespace() const;
  inline bool isSeparator() const;
  inline bool isString() const;
  inline bool isNumber() const;
  inline bool isLiteral() const;
  inline bool isDigit() const;
  inline bool isNumberExponentComponent() const;
  inline bool isEndOfFile() const;

  std::istream &_ss;
  size_t _position;
  bool _end = false;
  char _current;
};
} // namespace libjson
