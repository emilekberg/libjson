#pragma once
#include "json-types.h"
#include "lexer.h"
#include "token.h"
namespace libjson {

enum JsonParseFlags { NONE, ALLOW_TRAILING_COMMA };

JsonValue parse(std::istream &stream);
JsonObject parseObject(Lexer &lexer);
JsonValue parseValue(Lexer &lexer);
JsonValue parseNumber(const Token &token);
JsonValue parseLiteral(const Token &token);
JsonArray parseArray(Lexer &tokens);
} // namespace libjson
