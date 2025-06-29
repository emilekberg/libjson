#include <gtest/gtest.h>
#include <libjson/lexer.h>

TEST(Lexer, literal_true) {
   std::istringstream input(R"(true)");
   libjson::Lexer     lexer(input);
   libjson::Token     t = lexer.next();

   EXPECT_EQ(t.type, libjson::TokenTypes::LITERAL);
   EXPECT_EQ(t.literal, "true");
}

TEST(Lexer, literal_false) {
   std::istringstream input(R"(false)");

   libjson::Lexer lexer(input);
   libjson::Token t = lexer.next();

   EXPECT_EQ(t.type, libjson::TokenTypes::LITERAL);
   EXPECT_EQ(t.literal, "false");
}

TEST(Lexer, literal_null) {
   std::istringstream input(R"(null)");

   libjson::Lexer lexer(input);
   libjson::Token t = lexer.next();

   EXPECT_EQ(t.type, libjson::TokenTypes::LITERAL);
   EXPECT_EQ(t.literal, "null");
}
