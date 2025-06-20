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
  Lexer lexer(stream);
  if (lexer.peek() != Token_OpenBracket && lexer.peek() != Token_OpenBracer) {
    std::string err =
        std::format("expected {} or {}, but got {}", Token_OpenBracket.literal,
                    Token_OpenBracer.literal, lexer.peek().literal);
    throw std::invalid_argument(err);
  }

  JsonValue result = parseValue(lexer);
  Token eofToken = lexer.next();
  if (eofToken != Token_EndOfFile) {
    throw std::invalid_argument(std::format(
        "Unexpected Token after parsing data. Expected EOF but got {}",
        eofToken.literal));
  }
  return result;
}

JsonObject parseObject(Lexer &lexer) {
  JsonObjectData data;
  if (lexer.peek() == Token_CloseBracer) {
    lexer.next();
    return {data};
  }

  while (true) {
    Token tKey = lexer.next();
    if (tKey.type != TokenTypes::STRING) {
      throw std::invalid_argument(
          std::format("expected {}, but got {}", "string", tKey.literal));
    }
    Token tColon = lexer.next();
    if (tColon != Token_Colon) {
      throw std::invalid_argument(std::format(
          "expected {}, but got {}", Token_Colon.literal, tColon.literal));
    }

    data.emplace(std::move(tKey.literal), parseValue(lexer));

    Token tEnd = lexer.next();
    if (tEnd == Token_CloseBracer) {
      break;
    }

    // support trailing comma
    if (tEnd == Token_Comma && lexer.peek() == Token_CloseBracer) {
      lexer.next();
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

JsonArray parseArray(Lexer &lexer) {
  JsonArrayData data;
  if (lexer.peek() == Token_CloseBracket) {
    lexer.next();
    return {data};
  }

  while (true) {
    data.emplace_back(parseValue(lexer));

    Token tEnd = lexer.next();
    if (tEnd == Token_CloseBracket) {
      return {data};
    }
    if (tEnd == Token_Comma && lexer.peek() == Token_CloseBracer) {
      // support trailing comma
      lexer.next();
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

JsonValue parseValue(Lexer &lexer) {
  Token token = lexer.next();
  switch (token.type) {
  case TokenTypes::STRING:
    return {token.literal};

  case TokenTypes::NUMBER:
    return parseNumber(token);

  case TokenTypes::LITERAL:
    return parseLiteral(token);

  case TokenTypes::LEFT_BRACE:
    return {parseObject(lexer)};

  case TokenTypes::LEFT_BRACKET:
    return {parseArray(lexer)};

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
