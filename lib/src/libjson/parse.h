#pragma once
#include "json-types.h"
#include "token.h"
#include "tokenizer.h"
namespace libjson {

JsonValue parse(std::istream &stream);
JsonObject parseObject(LazyTokenizer &tokens);
JsonValue parseValue(LazyTokenizer &tokens);
JsonValue parseNumber(const Token &token);
JsonValue parseLiteral(const Token &token);
JsonArray parseArray(LazyTokenizer &tokens);
} // namespace libjson
