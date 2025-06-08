#include "parse.h"
#include "json-array.h"
#include "json-null.h"
#include "json-number.h"
#include "json-object.h"
#include "json-value.h"
#include "lexer.h"
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
  Token token = tokens.next();
  if (token != Token_OpenArray && token != Token_OpenBracket) {
    std::string err =
        std::format("expected {} or {}, but got {}", Token_OpenArray.literal,
                    Token_OpenBracket.literal, token.literal);
    throw std::invalid_argument(err);
  }
  JSONValue result = parseValue(token, tokens);
  if (tokens.peek() != Token_EndOfFile) {
    throw std::invalid_argument(std::format(
        "Unexpected Token after parsing data. Expected {} but got {}",
        Token_EndOfFile.literal, tokens.peek().literal));
  }
  return result;
}

JSONObject parseObject(Tokenizer &tokens) {
  JSONObject result;
  if (tokens.peek() == Token_CloseBracket) {
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
    JSONValue value = parseValue(tValue, tokens);
    result.add(std::string(tKey.literal), value);

    Token tEnd = tokens.next();
    if (tEnd == Token_CloseBracket) {
      return result;
    }
    if (tEnd == Token_Comma && tokens.peek() == Token_CloseBracket) {
      // support trailing comma
      // discard
      tokens.next();
      return result;
    }
    if (tEnd != Token_Comma) {
      throw std::invalid_argument(std::format(
          "parseObject: UnexpectedToken. Expected {} or {}, but got {}",
          Token_Comma.literal, Token_CloseBracket.literal, tEnd.literal));
    }
  }
  return result;
}

JSONArray parseArray(Tokenizer &tokens) {
  JSONArray result;
  if (tokens.peek() == Token_CloseArray) {
    tokens.next();
    return result;
  }

  while (true) {
    Token token = tokens.next();
    JSONValue value = parseValue(token, tokens);

    result.add(value);

    Token tEnd = tokens.next();
    if (tEnd == Token_CloseArray) {
      return result;
    }
    if (tEnd == Token_Comma && tokens.peek() == Token_CloseBracket) {
      // support trailing comma
      // discard
      tokens.next();
      return result;
    }
    if (tEnd != Token_Comma) {
      throw std::invalid_argument(std::format(
          "parseArray: Unexpected Token. Expected {} or {}, but got {}",
          Token_Comma.literal, Token_CloseArray.literal, tEnd.literal));
    }
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
    return {JSONValueType::_NULL, JSONNull{}};
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
      throw std::invalid_argument(std::format(
          "ParseJSONObject: Unexpected separator: {}.", token.literal));
    }
  } break;
  case TokenTypes::ILLEGAL:
    throw std::invalid_argument("ParseJSONObject: Reached illegal token");
  case TokenTypes::END_OF_FILE:
    throw std::invalid_argument(
        "ParseJSONObject: Reached EOF token when not expecting to.");
  }
  throw std::invalid_argument("ParseJSONObject: Should never reach here.");
}

Tokenizer tokenize(const std::string_view &input) {
  std::vector<Token> tokens;
  Lexer lexer(input);
  Token token;
  while (true) {
    token = lexer.next();
    // std::cout << token.literal << std::endl;
    if (token.type == TokenTypes::ILLEGAL) {
      throw std::invalid_argument(
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
