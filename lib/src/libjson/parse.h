#pragma once
#include "json-types.h"
#include "token.h"
#include "tokenizer.h"
#include <string_view>
namespace libjson {

JsonValue parse(const std::string_view &input);
JsonObject parseObject(LazyTokenizer &tokens);
JsonValue parseValue(LazyTokenizer &tokens);
JsonValue parseNumber(const Token &token);
JsonValue parseLiteral(const Token &token);
JsonArray parseArray(LazyTokenizer &tokens);
} // namespace libjson
