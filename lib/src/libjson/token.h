#pragma once
#include "token_types.h"
#include <string>

namespace libjson {

struct Token {
  Token(TokenTypes t) : type(t) {};
  Token(TokenTypes t, const std::string &l) : type(t), literal(std::move(l)) {}
  bool operator==(const Token &other) const { return type == other.type; }

  TokenTypes type;
  std::string literal;
};

static const Token Token_CurlyBraceOpen = {TokenTypes::CURLY_BRACE_OPEN, "{"};
static const Token Token_CurlyBraceClose = {TokenTypes::CURLY_BRACE_CLOSE, "}"};
static const Token Token_SquareBracketOpen = {TokenTypes::SQUARE_BRACKET_OPEN,
                                              "["};
static const Token Token_SquareBracketClose = {TokenTypes::SQUARE_BRACKET_CLOSE,
                                               "]"};
static const Token Token_Comma = {TokenTypes::COMMA, ","};
static const Token Token_Colon = {TokenTypes::COLON, ":"};
static const Token Token_String = {TokenTypes::STRING, "\" (double-quote)"};
static const Token Token_Number = {TokenTypes::NUMBER, "number"};
static const Token Token_Literal = {TokenTypes::LITERAL, "true,false, or null"};
static const Token Token_EndOfFile = {TokenTypes::END_OF_FILE, "EOF"};
static const Token Token_End = {TokenTypes::END, "\0"};

} // namespace libjson
