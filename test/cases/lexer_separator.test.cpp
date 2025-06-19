#include <gtest/gtest.h>
#include <libjson/lexer.h>

TEST(Lexer, separator_brackets) {
  std::istringstream input(R"({})");
  libjson::Lexer lexer(input);
  libjson::Token t_openbracket = lexer.next();
  libjson::Token t_closebracket = lexer.next();

  EXPECT_EQ(t_openbracket.type, libjson::TokenTypes::LEFT_BRACE);

  EXPECT_EQ(t_closebracket.type, libjson::TokenTypes::RIGHT_BRACE);
}

TEST(Lexer, separator_array_bracket) {
  std::istringstream input(R"([])");
  libjson::Lexer lexer(input);
  libjson::Token t_openbracket = lexer.next();
  libjson::Token t_closebracket = lexer.next();

  EXPECT_EQ(t_openbracket.type, libjson::TokenTypes::LEFT_BRACKET);

  EXPECT_EQ(t_closebracket.type, libjson::TokenTypes::RIGHT_BRACKET);
}

TEST(Lexer, separator_colon) {
  std::istringstream input(R"(:)");
  libjson::Lexer lexer(input);
  libjson::Token t = lexer.next();

  EXPECT_EQ(t.type, libjson::TokenTypes::COLON);
}

TEST(Lexer, separator_comma) {
  std::istringstream input(R"(,)");
  libjson::Lexer lexer(input);
  libjson::Token t = lexer.next();

  EXPECT_EQ(t.type, libjson::TokenTypes::COMMA);
}
