#pragma once
#include "token_types.h"
#include <string>
#include <string_view>

namespace libjson {

struct Token {
  bool operator==(const Token &other) const {
    return type == other.type && literal == other.literal;
  }

  TokenTypes type;
  std::string_view literal;
};

static const Token Token_OpenBracket = {TokenTypes::SEPARATOR, "{"};
static const Token Token_CloseBracket = {TokenTypes::SEPARATOR, "}"};
static const Token Token_OpenArray = {TokenTypes::SEPARATOR, "["};
static const Token Token_CloseArray = {TokenTypes::SEPARATOR, "]"};
static const Token Token_Comma = {TokenTypes::SEPARATOR, ","};
static const Token Token_Colon = {TokenTypes::SEPARATOR, ":"};
static const Token Token_EndOfFile = {TokenTypes::END_OF_FILE, "\0"};

} // namespace libjson
