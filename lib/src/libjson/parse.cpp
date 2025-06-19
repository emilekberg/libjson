#include "parse.h"
#include "json-types.h"
#include "token.h"
#include "token_types.h"
#include <cassert>
#include <format>
#include <stdexcept>
#include <string_view>
namespace libjson {

JsonValue parse(std::istream &stream) {
  LazyTokenizer tokens(stream);
  if (tokens.peek() != Token_OpenBracket && tokens.peek() != Token_OpenBracer) {
    std::string err =
        std::format("expected {} or {}, but got {}", Token_OpenBracket.literal,
                    Token_OpenBracer.literal, tokens.peek().literal);
    throw std::invalid_argument(err);
  }

  JsonValue result = parseValue(tokens);
  Token eofToken = tokens.next();
  if (eofToken != Token_EndOfFile) {
    throw std::invalid_argument(std::format(
        "Unexpected Token after parsing data. Expected EOF but got {}",
        eofToken.literal));
  }
  return result;
}

JsonObject parseObject(LazyTokenizer &tokens) {
  JsonObjectData data;
  if (tokens.peek() == Token_CloseBracer) {
    tokens.next();
    return {data};
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

    data.emplace(std::string(tKey.literal), parseValue(tokens));

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
  return {data};
}

JsonArray parseArray(LazyTokenizer &tokens) {
  JsonArrayData data;
  if (tokens.peek() == Token_CloseBracket) {
    tokens.next();
    return {data};
  }

  while (true) {
    data.emplace_back(parseValue(tokens));

    Token tEnd = tokens.next();
    if (tEnd == Token_CloseBracket) {
      return {data};
    }
    if (tEnd == Token_Comma && tokens.peek() == Token_CloseBracer) {
      // support trailing comma
      tokens.next();
      return {data};
    }
    if (tEnd != Token_Comma) {
      throw std::invalid_argument(std::format(
          "parseArray: Unexpected Token. Expected {} or {}, but got {}",
          Token_Comma.literal, Token_CloseBracket.literal, tEnd.literal));
    }
  }
  return {data};
}

JsonValue parseNumber(const Token &token) {
  return {JsonNumber(std::string(token.literal))};
}

JsonValue parseLiteral(const Token &token) {
  switch (token.literal[0]) {
  case 't':
    return {true};
  case 'f':
    return {false};
  case 'n':
    return {nullptr};
  }
  throw std::runtime_error("should never come here");
}

JsonValue parseValue(LazyTokenizer &tokens) {
  Token token = tokens.next();
  switch (token.type) {
  case TokenTypes::STRING:
    return {std::string(token.literal)};

  case TokenTypes::NUMBER:
    return parseNumber(token);

  case TokenTypes::LITERAL:
    return parseLiteral(token);

  case TokenTypes::LEFT_BRACE:
    return {parseObject(tokens)};
  case libjson::TokenTypes::LEFT_BRACKET:
    return {parseArray(tokens)};

  case TokenTypes::ILLEGAL:
    throw std::invalid_argument("ParseJsonValue: Reached illegal token");

  case TokenTypes::END_OF_FILE:
    throw std::invalid_argument(
        "ParseJsonValue: Reached EOF token when not expecting to.");

  case TokenTypes::HEAD:
  case TokenTypes::END:
    throw std::invalid_argument(
        "ParseJsonValue: Unexpected token type HEAD or NONE.");
  case TokenTypes::RIGHT_BRACE:
  case TokenTypes::RIGHT_BRACKET:
  case TokenTypes::COMMA:
  case TokenTypes::COLON:
    throw std::invalid_argument(std::format(
        "ParseJsonValue: Unexpected separator: {}.", token.literal));
  }
  throw std::invalid_argument("ParseJsonValue: Should never reach here.");
}
} // namespace libjson
