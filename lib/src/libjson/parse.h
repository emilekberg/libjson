#pragma once
#include "json-array.h"
#include "json-object.h"
#include "token.h"
#include <string_view>
#include <vector>
namespace libjson {

struct Tokenizer {
  Token &next() { return tokens[pos++]; }
  std::vector<Token> tokens;
  size_t pos{};
};

JSONValue parse(const std::string_view &input);
JSONObject parseObject(Tokenizer &tokens);
JSONValue parseValue(const Token &token, Tokenizer &tokens);
JSONValue parseNumber(const Token &token);
JSONValue parseLiteral(const Token &token);
JSONArray parseArray(Tokenizer &tokens);
Tokenizer tokenize(const std::string_view &input);
} // namespace libjson
