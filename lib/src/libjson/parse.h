#pragma once
#include "json-array.h"
#include "json-object.h"
#include "token.h"
#include "tokenizer.h"
#include <string_view>
namespace libjson {

JSONValue parse(const std::string_view &input);
JSONObject parseObject(LazyTokenizer &tokens);
JSONValue parseValue(const Token &token, LazyTokenizer &tokens);
JSONValue parseNumber(const Token &token);
JSONValue parseLiteral(const Token &token);
JSONArray parseArray(LazyTokenizer &tokens);
} // namespace libjson
