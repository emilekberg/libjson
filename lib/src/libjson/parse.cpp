#include "parse.h"
#include "exceptions.h"
#include "json-types.h"
#include "token.h"
#include "token_types.h"
#include <cassert>
#include <stdexcept>
namespace libjson {

JsonValue parse(std::istream &stream) {
  Lexer lexer(stream);
  expect_tokens(lexer.peek(), Token_OpenBracer, Token_OpenBracket);
  JsonValue result = parseValue(lexer);
  expect_token(lexer.next(), Token_EndOfFile);
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
    expect_token(tKey, Token_String);
    expect_token(lexer.next(), Token_Colon);

    data.emplace(tKey.literal, parseValue(lexer));

    Token tEnd = lexer.next();
    if (tEnd == Token_CloseBracer) {
      break;
    }

    // support trailing comma
    if (tEnd == Token_Comma && lexer.peek() == Token_CloseBracer) {
      lexer.next();
      break;
    }
    expect_token(tEnd, Token_Comma);
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
    expect_token(tEnd, Token_Comma);
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

  case TokenTypes::CURLY_BRACE_OPEN:
    return {parseObject(lexer)};

  case TokenTypes::SQUARE_BRACKET_OPEN:
    return {parseArray(lexer)};

  case TokenTypes::ILLEGAL:
    throw unexpected_token(token.literal, "Not Illegal Token");

  case TokenTypes::END_OF_FILE:
    throw unexpected_token(token.literal, "Not EOF Token");

  case TokenTypes::HEAD:
  case TokenTypes::END:
    throw unexpected_token(token.literal, "Not HEAD or END Token");
  case TokenTypes::SQUARE_BRACKET_CLOSE:
  case TokenTypes::CURLY_BRACE_CLOSE:
  case TokenTypes::COMMA:
  case TokenTypes::COLON:
    throw unexpected_token(token.literal, "Unexpected Separator");
  }
  throw std::invalid_argument("ParseJsonValue: Should never reach here.");
}
} // namespace libjson
