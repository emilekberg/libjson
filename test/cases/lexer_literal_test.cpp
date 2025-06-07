
#include "libjson/token.h"
#include "libjson/token_types.h"
#include <gtest/gtest.h>
#include <libjson/lexer.h>

TEST(Lexer, literal_true) {
  libjson::Lexer lexer(R"(true)");
  libjson::Token t = lexer.next();

  EXPECT_EQ(t.type, libjson::TokenTypes::LITERAL);
  EXPECT_EQ(t.literal, "true");
}

TEST(Lexer, literal_false) {
  libjson::Lexer lexer(R"(false)");
  libjson::Token t = lexer.next();

  EXPECT_EQ(t.type, libjson::TokenTypes::LITERAL);
  EXPECT_EQ(t.literal, "false");
}

TEST(Lexer, literal_null) {
  libjson::Lexer lexer(R"(null)");
  libjson::Token t = lexer.next();

  EXPECT_EQ(t.type, libjson::TokenTypes::LITERAL);
  EXPECT_EQ(t.literal, "null");
}
