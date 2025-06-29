#include <gtest/gtest.h>
#include <libjson/exceptions.h>
#include <libjson/lexer.h>

TEST(Lexer, number) {
   std::istringstream input(R"(1337 )");
   libjson::Lexer     lexer(input);
   libjson::Token     t = lexer.next();

   EXPECT_EQ(t.type, libjson::TokenTypes::NUMBER);
   EXPECT_EQ(t.literal, "1337");
}
TEST(Lexer, number_with_dot) {
   std::istringstream input(R"(420.69 )");

   libjson::Lexer lexer(input);
   libjson::Token t = lexer.next();

   EXPECT_EQ(t.type, libjson::TokenTypes::NUMBER);
   EXPECT_EQ(t.literal, "420.69");
}

TEST(Lexer, number_negative) {
   std::istringstream input(R"(-88 )");

   libjson::Lexer lexer(input);
   libjson::Token t = lexer.next();

   EXPECT_EQ(t.type, libjson::TokenTypes::NUMBER);
   EXPECT_EQ(t.literal, "-88");
}

TEST(Lexer, number_negative_with_dot) {
   std::istringstream input(R"(-32.15 )");

   libjson::Lexer lexer(input);
   libjson::Token t = lexer.next();

   EXPECT_EQ(t.type, libjson::TokenTypes::NUMBER);
   EXPECT_EQ(t.literal, "-32.15");
}

TEST(Lexer, number_invalid_if_multiple_dots) {
   std::istringstream input(R"(142.12.152 )");

   libjson::Lexer lexer(input);

   EXPECT_THROW(lexer.next(), libjson::unexpected_token);
   // libjson::Token t = lexer.next();
   // EXPECT_EQ(t.type, libjson::TokenTypes::ILLEGAL);
}
