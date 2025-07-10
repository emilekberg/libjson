#include "parse.h"
#include "exceptions.h"
#include "json-types.h"
#include "token-types.h"
#include "token.h"
namespace libjson {

JsonValue parse(std::istream &stream) {
   Lexer lexer(stream);
   expect_tokens(lexer.peek(), Token_CurlyBraceOpen, Token_SquareBracketOpen);
   JsonValue result = parseValue(lexer);
   expect_token(lexer.next(), Token_EndOfFile);
   return result;
}

JsonObject parseObject(Lexer &lexer) {
   if (lexer.peek() == Token_CurlyBraceClose) {
      lexer.next();
      return JsonObject{};
   }

   JsonObjectData data;
   data.reserve(16);
   while (true) {
      Token tKey = lexer.next();
      expect_token(tKey, Token_String);
      expect_token(lexer.next(), Token_Colon);

      data.emplace(tKey.literal, parseValue(lexer));

      Token tEnd = lexer.next();
      if (tEnd == Token_CurlyBraceClose) {
         break;
      } else if (tEnd == Token_Comma && lexer.peek() == Token_CurlyBraceClose) {
         // discard trailing comma
         lexer.next();
         break;
      }
      expect_token(tEnd, Token_Comma);
   }
   return JsonObject{std::move(data)};
}

JsonArray parseArray(Lexer &lexer) {
   if (lexer.peek() == Token_SquareBracketClose) {
      lexer.next();
      return JsonArray{};
   }

   JsonArrayData data;
   data.reserve(16);
   while (true) {
      data.emplace_back(parseValue(lexer));

      Token tEnd = lexer.next();
      if (tEnd == Token_SquareBracketClose) {
         break;
      } else if (tEnd == Token_Comma && lexer.peek() == Token_CurlyBraceClose) {
         // skip trailing comma
         // TODO: check this with mode flag.
         // (flags & OPTS_TRAILING_COMMA) != 0
         lexer.next();
         break;
      }
      expect_token(tEnd, Token_Comma);
   }
   return JsonArray{std::move(data)};
}

JsonValue parseNumber(const Token &token) {
   return {JsonNumber(token.literal)};
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
   throw unexpected_token(token.literal, Token_Literal.literal);
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

   default:
      throw unexpected_token::from_variadic(
          token.literal, Token_String, Token_Number, Token_Literal, Token_CurlyBraceOpen,
          Token_SquareBracketOpen
      );
   }
}
} // namespace libjson
