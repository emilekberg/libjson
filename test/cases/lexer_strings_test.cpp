
#include "libjson/token.h"
#include "libjson/token_types.h"
#include <gtest/gtest.h>
#include <libjson/lexer.h>

TEST(Lexer, string_quotes) {
  libjson::Lexer lexer(R"("string with quotes")");
  libjson::Token t = lexer.next();
  EXPECT_EQ(t.type, libjson::TokenTypes::STRING);
  EXPECT_EQ(t.literal, "string with quotes");
}

TEST(Lexer, string_single) {
  libjson::Lexer lexer(R"('string with single')");
  libjson::Token t = lexer.next();
  EXPECT_EQ(t.type, libjson::TokenTypes::STRING);
  EXPECT_EQ(t.literal, "string with single");
}

TEST(Lexer, string_quotes_can_contain_single) {
  libjson::Lexer lexer(R"("has 'single'")");
  libjson::Token t = lexer.next();
  EXPECT_EQ(t.type, libjson::TokenTypes::STRING);
  EXPECT_EQ(t.literal, "has 'single'");
}

TEST(Lexer, string_single_can_contain_quote) {
  libjson::Lexer lexer(R"('has "quote"')");
  libjson::Token t = lexer.next();
  EXPECT_EQ(t.type, libjson::TokenTypes::STRING);
  EXPECT_EQ(t.literal, "has \"quote\"");
}

TEST(Lexer, string_can_escape) {
  libjson::Lexer lexer(R"("string can contain \"escaped\" substrings")");
  libjson::Token t = lexer.next();
  EXPECT_EQ(t.type, libjson::TokenTypes::STRING);
  EXPECT_EQ(t.literal, "string can contain \\\"escaped\\\" substrings");
}
