#include "parse.h"
#include "json-array.h"
#include "json-value.h"
#include "lexer.h"
#include "libjson/json-number.h"
#include "token.h"
#include "token_types.h"
#include <cassert>
#include <format>
#include <stdexcept>
#include <string_view>
#include <vector>
namespace libjson {
JSONValue parse(const std::string_view &input) {
  Tokenizer tokens = tokenize(input);
  JSONObject result;
  Token t = tokens.next();
  return parseValue(t, tokens);
}

JSONObject parseObject(Tokenizer &tokens) {
  JSONObject result;
  while (true) {
    Token tKey = tokens.next();
    assert(tKey.type == TokenTypes::STRING);
    Token tColon = tokens.next();
    assert(tColon == Token_Colon);

    Token tValue = tokens.next();
    JSONValue value = parseValue(tValue, tokens);
    result.add(std::string(tKey.literal), value);

    Token tEnd = tokens.next();
    if (tEnd == Token_CloseBracket) {
      return result;
    }
    assert(tEnd == Token_Comma);
  }
  return result;
}

JSONArray parseArray(Tokenizer &tokens) {
  JSONArray result;

  while (true) {
    Token token = tokens.next();
    JSONValue value = parseValue(token, tokens);

    result.add(value);

    Token tEnd = tokens.next();
    if (tEnd == Token_CloseArray) {
      return result;
    }
    assert(tEnd == Token_Comma);
  }

  return result;
}

JSONValue parseNumber(const Token &token) {
  return {JSONValueType::NUMBER, JSONNumber(std::string(token.literal))};
}

JSONValue parseLiteral(const Token &token) {
  switch (token.literal[0]) {
  case 't':
    return {JSONValueType::BOOL, true};
  case 'f':
    return {JSONValueType::BOOL, false};
  case 'n':
    return {JSONValueType::_NULL, nullptr};
  }
  throw std::runtime_error("should never come here");
}

JSONValue parseValue(const Token &token, Tokenizer &tokens) {
  switch (token.type) {
  case TokenTypes::STRING: {
    return {JSONValueType::STRING, std::string(token.literal)};
  } break;
  case TokenTypes::NUMBER: {
    return parseNumber(token);
  } break;
  case TokenTypes::LITERAL: {
    return parseLiteral(token);
  } break;
  case TokenTypes::SEPARATOR: {
    if (token == Token_OpenBracket) {
      return {JSONValueType::OBJECT, parseObject(tokens)};
    } else if (token == Token_OpenArray) {
      return {JSONValueType::ARRAY, parseArray(tokens)};
    } else {
      throw std::runtime_error("ParseJSONObject: Unexpected separator.");
    }
  } break;
  case TokenTypes::ILLEGAL:
    throw std::runtime_error("ParseJSONObject: Reached illegal token");
  case TokenTypes::END_OF_FILE:
    throw std::runtime_error(
        "ParseJSONObject: Reached EOF token when not expecting to.");
  }
  throw std::runtime_error("ParseJSONObject: Should never reach here.");
}

Tokenizer tokenize(const std::string_view &input) {
  std::vector<Token> tokens;
  Lexer lexer(input);
  Token token;
  while (true) {
    token = lexer.next();
    if (token.type == TokenTypes::ILLEGAL) {
      throw std::runtime_error(
          std::format("ILLEGAL TOKEN FOUND: {}", token.literal));
    }
    tokens.push_back(token);
    if (token.type == libjson::TokenTypes::END_OF_FILE) {
      break;
    }
  }
  return {tokens};
}
} // namespace libjson
