#include "parse.h"
#include "lexer.h"
#include "libjson/json-array.h"
#include "libjson/json-value.h"
#include "token.h"
#include "token_types.h"
#include <cassert>
#include <stdexcept>
#include <vector>
namespace libjson {
JSONObject parse(const std::string &input) {
  Tokenizer tokens = tokenize(input);
  JSONObject result;
  Token t = tokens.next();
  switch (t.type) {
  case TokenTypes::SEPARATOR: {
    if (t.literal == "{") {
      return parseObject(tokens);
    } else if (t.literal == "[") {
      throw std::runtime_error("not implemented");
    }
  } break;
  case TokenTypes::ILLEGAL:
  case TokenTypes::END_OF_FILE:
  default:
    return result;
  }
  return result;
}

JSONObject parseObject(Tokenizer &tokens) {
  JSONObject result;
  while (true) {
    Token tKey = tokens.next();
    assert(tKey.type == TokenTypes::STRING);
    Token tColon = tokens.next();
    assert(tColon.type == TokenTypes::SEPARATOR);
    assert(tColon.literal == ":");

    Token tValue = tokens.next();
    JSONValue value = parseValue(tValue, tokens);
    result.add(tKey.literal, value);

    Token tEnd = tokens.next();
    if (tEnd.type == TokenTypes::SEPARATOR && tEnd.literal == "}") {
      return result;
    }

    assert(tEnd.type == TokenTypes::SEPARATOR);
    assert(tEnd.literal == ",");
  }

  return result;
}

JSONArray parseArray(Tokenizer &tokens) {
  JSONArray result;

  while (true) {
    Token token = tokens.next();
    JSONValue value = parseValue(token, tokens);

    result.data.push_back(value);

    Token tEnd = tokens.next();
    if (tEnd.type == TokenTypes::SEPARATOR && tEnd.literal == "}") {
      return result;
    }
    assert(tEnd.type == TokenTypes::SEPARATOR);
    assert(tEnd.literal == ",");
  }

  return result;
}

JSONValue parseNumber(const Token &token) {
  double d = std::stod(token.literal);
  return {JSONValueType::NUMBER, d};
}

JSONValue parseLiteral(const Token &token) {
  switch (token.literal[0]) {
  case 't':
    return {JSONValueType::BOOL, true};
  case 'f':
    return {JSONValueType::BOOL, false};
    break;
  case 'n':
    return {JSONValueType::_NULL, nullptr};
    break;
  }
  throw std::runtime_error("should never come here");
}

JSONValue parseValue(const Token &token, Tokenizer &tokens) {
  switch (token.type) {
  case TokenTypes::STRING: {
    return {JSONValueType::STRING, token.literal};
  } break;
  case TokenTypes::NUMBER: {
    return parseNumber(token);
  } break;
  case TokenTypes::LITERAL: {
    return parseLiteral(token);
  } break;
  case TokenTypes::SEPARATOR: {
    if (token.literal == "{") {
      return {JSONValueType::OBJECT, parseObject(tokens)};
    } else if (token.literal == "[") {
      return {JSONValueType::ARRAY, parseArray(tokens)};
    }
  } break;
  case TokenTypes::ILLEGAL:
  case TokenTypes::END_OF_FILE:
  default:
    throw std::runtime_error("ParseJSONObject: Reached unnexpected token");
    break;
  }
  throw std::runtime_error("ParseJSONObject: Reached unnexpected token");
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
