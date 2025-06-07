#include "parse.h"
#include "libjson/lexer.h"
#include "libjson/object.h"
#include "libjson/token_types.h"
#include "token.h"
#include <cassert>
#include <stdexcept>
#include <vector>
namespace libjson {
Object parse(const std::string &input) {
  Tokenizer tokens = tokenize(input);
  Object result;
  Token t = tokens.next();
  assert(t.type == TokenTypes::SEPARATOR && t.literal == "{");

  t = tokens.next();
  assert(t.type == TokenTypes::STRING);
  std::string key = t.literal;

  t = tokens.next();
  assert(t.type == TokenTypes::SEPARATOR && t.literal == ":");

  t = tokens.next();
  assert(t.type == TokenTypes::STRING);
  std::any value = t.literal;
  result.add(key, {ValueType::STRING, value});

  t = tokens.next();
  assert(t.type == TokenTypes::SEPARATOR && t.literal == "}");

  t = tokens.next();
  assert(t.type == TokenTypes::END_OF_FILE);

  return result;
}
Tokenizer tokenize(const std::string &input) {
  std::vector<Token> tokens;
  Lexer lexer(input);
  Token token;
  while (true) {
    token = lexer.next();
    if (token.type == TokenTypes::ILLEGAL) {
      throw std::runtime_error("ILLEGAL TOKEN FOUND");
    }
    tokens.push_back(token);
    if (token.type == libjson::TokenTypes::END_OF_FILE) {
      break;
    }
  }
  return {tokens};
}
} // namespace libjson
