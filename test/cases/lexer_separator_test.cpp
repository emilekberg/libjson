#include "libjson/token.h"
#include "libjson/token_types.h"
#include <gtest/gtest.h>
#include <libjson/lexer.h>

TEST(Lexer, separator_brackets) {
  libjson::Lexer lexer(R"({})");
  libjson::Token t_openbracket = lexer.next();
  libjson::Token t_closebracket = lexer.next();

  EXPECT_EQ(t_openbracket.type, libjson::TokenTypes::SEPARATOR);
  EXPECT_EQ(t_openbracket.literal, "{");

  EXPECT_EQ(t_closebracket.type, libjson::TokenTypes::SEPARATOR);
  EXPECT_EQ(t_closebracket.literal, "}");
}

TEST(Lexer, separator_array_bracket) {
  libjson::Lexer lexer(R"([])");
  libjson::Token t_openbracket = lexer.next();
  libjson::Token t_closebracket = lexer.next();

  EXPECT_EQ(t_openbracket.type, libjson::TokenTypes::SEPARATOR);
  EXPECT_EQ(t_openbracket.literal, "[");

  EXPECT_EQ(t_closebracket.type, libjson::TokenTypes::SEPARATOR);
  EXPECT_EQ(t_closebracket.literal, "]");
}

TEST(Lexer, separator_colon) {
  libjson::Lexer lexer(R"(:)");
  libjson::Token t = lexer.next();

  EXPECT_EQ(t.type, libjson::TokenTypes::SEPARATOR);
  EXPECT_EQ(t.literal, ":");
}

TEST(Lexer, separator_comma) {
  libjson::Lexer lexer(R"(,)");
  libjson::Token t = lexer.next();

  EXPECT_EQ(t.type, libjson::TokenTypes::SEPARATOR);
  EXPECT_EQ(t.literal, ",");
}
