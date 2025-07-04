#include "parse.h"
#include "exceptions.h"
#include "json-types.h"
#include "token.h"
#include "token_types.h"
namespace libjson {

JsonValue parse(std::istream &stream) {
   Lexer lexer(stream);
   expect_tokens(lexer.peek(), Token_CurlyBraceOpen, Token_SquareBracketOpen);
   JsonValue result = parseValue(lexer);
   expect_token(lexer.next(), Token_EndOfFile);
   return result;
}

JsonObject parseObject(Lexer &lexer) {
   JsonObjectData data;
   data.reserve(16);
   if (lexer.peek() == Token_CurlyBraceClose) {
      lexer.next();
      return {data};
   }

   while (true) {
      Token tKey = lexer.next();
      expect_token(tKey, Token_String);
      expect_token(lexer.next(), Token_Colon);

      data.emplace(tKey.literal, parseValue(lexer));

      Token tEnd = lexer.next();
      if (tEnd == Token_CurlyBraceClose) {
         break;
      }

      if (tEnd == Token_Comma && lexer.peek() == Token_CurlyBraceClose) {
         // discard trailing comma
         lexer.next();
         break;
      }
      expect_token(tEnd, Token_Comma);
   }
   return {data};
}

JsonArray parseArray(Lexer &lexer) {
   JsonArrayData data;
   data.reserve(16);
   if (lexer.peek() == Token_SquareBracketClose) {
      lexer.next();
      return {data};
   }

   while (true) {
      data.emplace_back(parseValue(lexer));

      Token tEnd = lexer.next();
      if (tEnd == Token_SquareBracketClose) {
         return {data};
      }

      if (tEnd == Token_Comma && lexer.peek() == Token_CurlyBraceClose) {
         // support trailing comma
         lexer.next();
         return {data};
      }
      expect_token(tEnd, Token_Comma);
   }
   return {data};
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
