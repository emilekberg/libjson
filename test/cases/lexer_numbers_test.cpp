#include "libjson/token.h"
#include "libjson/token_types.h"
#include <gtest/gtest.h>
#include <libjson/lexer.h>

TEST(Lexer, number) {
  libjson::Lexer lexer(R"(1337)");
  libjson::Token t = lexer.next();

  EXPECT_EQ(t.type, libjson::TokenTypes::NUMBER);
  EXPECT_EQ(t.literal, "1337");
}
TEST(Lexer, number_with_dot) {
  libjson::Lexer lexer(R"(420.69)");
  libjson::Token t = lexer.next();

  EXPECT_EQ(t.type, libjson::TokenTypes::NUMBER);
  EXPECT_EQ(t.literal, "420.69");
}

TEST(Lexer, number_negative) {
  libjson::Lexer lexer(R"(-88)");
  libjson::Token t = lexer.next();

  EXPECT_EQ(t.type, libjson::TokenTypes::NUMBER);
  EXPECT_EQ(t.literal, "-88");
}

TEST(Lexer, number_negative_with_dot) {
  libjson::Lexer lexer(R"(-32.15)");
  libjson::Token t = lexer.next();

  EXPECT_EQ(t.type, libjson::TokenTypes::NUMBER);
  EXPECT_EQ(t.literal, "-32.15");
}

TEST(Lexer, number_invalid_if_multiple_dots) {
  libjson::Lexer lexer(R"(142.12.152)");
  libjson::Token t = lexer.next();

  EXPECT_EQ(t.type, libjson::TokenTypes::ILLEGAL);
  EXPECT_EQ(t.literal, "142.12.152");
}
