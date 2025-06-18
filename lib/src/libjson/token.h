#pragma once
#include "token_types.h"
#include <string>

namespace libjson {

struct Token {
  bool operator==(const Token &other) const {
    return type == other.type && literal == other.literal;
  }

  TokenTypes type;
  std::string literal;
};

// static const Token Token_StartOfFile = {TokenTypes::HEAD, ""};
static const Token Token_OpenBracer = {TokenTypes::SEPARATOR, "{"};
static const Token Token_CloseBracer = {TokenTypes::SEPARATOR, "}"};
static const Token Token_OpenBracket = {TokenTypes::SEPARATOR, "["};
static const Token Token_CloseBracket = {TokenTypes::SEPARATOR, "]"};
static const Token Token_Comma = {TokenTypes::SEPARATOR, ","};
static const Token Token_Colon = {TokenTypes::SEPARATOR, ":"};
static const Token Token_EndOfFile = {TokenTypes::END_OF_FILE, "\0"};
static const Token Token_None = {TokenTypes::NONE, "\0"};

} // namespace libjson
