#pragma once
#include "object.h"
#include "token.h"
#include <string>
#include <vector>
namespace libjson {

struct Tokenizer {
  Token &next() { return tokens[pos++]; }
  std::vector<Token> tokens;
  size_t pos{};
};

Object parse(const std::string &input);
Object parseObject(const Tokenizer &tokens);
std::vector<Value> parseArray(const Tokenizer &tokens);
Value parseValue(const Tokenizer &tokens);
Tokenizer tokenize(const std::string &input);
} // namespace libjson
