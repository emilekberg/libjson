#include "parse.h"
#include "json-array.h"
#include "json-null.h"
#include "json-number.h"
#include "json-object.h"
#include "json-value.h"
#include "token.h"
#include "token_types.h"
#include <cassert>
#include <format>
#include <stdexcept>
#include <string_view>
namespace libjson {
JsonValue parse(const std::string_view &input) {
  LazyTokenizer tokens(input);
  Token token = tokens.next();
  if (token != Token_OpenBracket && token != Token_OpenBracer) {
    std::string err =
        std::format("expected {} or {}, but got {}", Token_OpenBracket.literal,
                    Token_OpenBracer.literal, token.literal);
    throw std::invalid_argument(err);
  }
  JsonValue result = parseValue(token, tokens);
  if (tokens.peek() != Token_EndOfFile) {
    throw std::invalid_argument(std::format(
        "Unexpected Token after parsing data. Expected {} but got {}",
        Token_EndOfFile.literal, tokens.peek().literal));
  }
  return result;
}

JsonObject parseObject(LazyTokenizer &tokens) {
  JsonObject result;
  if (tokens.peek() == Token_CloseBracer) {
    tokens.next();
    return result;
  }
  while (true) {
    Token tKey = tokens.next();
    if (tKey.type != TokenTypes::STRING) {
      throw std::invalid_argument(
          std::format("expected {}, but got {}", "string", tKey.literal));
    }
    Token tColon = tokens.next();
    if (tColon != Token_Colon) {
      throw std::invalid_argument(std::format(
          "expected {}, but got {}", Token_Colon.literal, tColon.literal));
    }

    Token tValue = tokens.next();
    JsonValue value = parseValue(tValue, tokens);
    result.add(std::string(tKey.literal), value);

    Token tEnd = tokens.next();
    if (tEnd == Token_CloseBracer) {
      break;
    }

    // support trailing comma
    if (tEnd == Token_Comma && tokens.peek() == Token_CloseBracer) {
      tokens.next();
      break;
    }
    if (tEnd != Token_Comma) {
      throw std::invalid_argument(std::format(
          "parseObject: UnexpectedToken. Expected {} or {}, but got {}",
          Token_Comma.literal, Token_CloseBracer.literal, tEnd.literal));
    }
  }
  return result;
}

JsonArray parseArray(LazyTokenizer &tokens) {
  JsonArray result;
  if (tokens.peek() == Token_CloseBracket) {
    tokens.next();
    return result;
  }

  while (true) {
    Token token = tokens.next();
    JsonValue value = parseValue(token, tokens);
    result.add(value);

    Token tEnd = tokens.next();
    if (tEnd == Token_CloseBracket) {
      return result;
    }
    if (tEnd == Token_Comma && tokens.peek() == Token_CloseBracer) {
      // support trailing comma
      tokens.next();
      return result;
    }
    if (tEnd != Token_Comma) {
      throw std::invalid_argument(std::format(
          "parseArray: Unexpected Token. Expected {} or {}, but got {}",
          Token_Comma.literal, Token_CloseBracket.literal, tEnd.literal));
    }
  }

  return result;
}

JsonValue parseNumber(const Token &token) {
  return {JsonValueType::NUMBER, JsonNumber(std::string(token.literal))};
}

JsonValue parseLiteral(const Token &token) {
  switch (token.literal[0]) {
  case 't':
    return {JsonValueType::BOOL, true};
  case 'f':
    return {JsonValueType::BOOL, false};
  case 'n':
    return {JsonValueType::_NULL, JsonNull{}};
  }
  throw std::runtime_error("should never come here");
}

JsonValue parseValue(const Token &token, LazyTokenizer &tokens) {
  switch (token.type) {
  case TokenTypes::STRING:
    return {JsonValueType::STRING, std::string(token.literal)};

  case TokenTypes::NUMBER:
    return parseNumber(token);

  case TokenTypes::LITERAL:
    return parseLiteral(token);

  case TokenTypes::SEPARATOR:
    if (token == Token_OpenBracer) {
      return {JsonValueType::OBJECT, parseObject(tokens)};
    } else if (token == Token_OpenBracket) {
      return {JsonValueType::ARRAY, parseArray(tokens)};
    }
    throw std::invalid_argument(std::format(
        "ParseJsonValue: Unexpected separator: {}.", token.literal));

  case TokenTypes::ILLEGAL:
    throw std::invalid_argument("ParseJsonValue: Reached illegal token");

  case TokenTypes::END_OF_FILE:
    throw std::invalid_argument(
        "ParseJsonValue: Reached EOF token when not expecting to.");

  case TokenTypes::HEAD:
  case TokenTypes::NONE:
    throw std::invalid_argument(
        "ParseJsonValue: Unexpected token type HEAD or NONE.");
  }
  throw std::invalid_argument("ParseJsonValue: Should never reach here.");
}
} // namespace libjson
