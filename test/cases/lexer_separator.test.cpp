#include <gtest/gtest.h>
#include <libjson/lexer.h>

TEST(Lexer, separator_brackets) {
  std::istringstream input(R"({})");
  libjson::Lexer lexer(input);
  libjson::Token t_openbracket = lexer.next();
  libjson::Token t_closebracket = lexer.next();

  EXPECT_EQ(t_openbracket.type, libjson::TokenTypes::SEPARATOR);
  EXPECT_EQ(t_openbracket.literal, "{");

  EXPECT_EQ(t_closebracket.type, libjson::TokenTypes::SEPARATOR);
  EXPECT_EQ(t_closebracket.literal, "}");
}

TEST(Lexer, separator_array_bracket) {
  std::istringstream input(R"([])");
  libjson::Lexer lexer(input);
  libjson::Token t_openbracket = lexer.next();
  libjson::Token t_closebracket = lexer.next();

  EXPECT_EQ(t_openbracket.type, libjson::TokenTypes::SEPARATOR);
  EXPECT_EQ(t_openbracket.literal, "[");

  EXPECT_EQ(t_closebracket.type, libjson::TokenTypes::SEPARATOR);
  EXPECT_EQ(t_closebracket.literal, "]");
}

TEST(Lexer, separator_colon) {
  std::istringstream input(R"(:)");
  libjson::Lexer lexer(input);
  libjson::Token t = lexer.next();

  EXPECT_EQ(t.type, libjson::TokenTypes::SEPARATOR);
  EXPECT_EQ(t.literal, ":");
}

TEST(Lexer, separator_comma) {
  std::istringstream input(R"(,)");
  libjson::Lexer lexer(input);
  libjson::Token t = lexer.next();

  EXPECT_EQ(t.type, libjson::TokenTypes::SEPARATOR);
  EXPECT_EQ(t.literal, ",");
}
