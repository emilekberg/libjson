#include <gtest/gtest.h>
#include <libjson/lexer.h>

TEST(Lexer, skips_leading_whitespace) {
  libjson::Lexer lexer(R"(       "ignored leading whitespace"      )");
  libjson::Token t = lexer.next();
  EXPECT_EQ(t.type, libjson::TokenTypes::STRING);
  EXPECT_EQ(t.literal, "ignored leading whitespace");
}

TEST(Lexer, skips_middle_whitespace) {
  libjson::Lexer lexer(
      R"(       "we've had one string yes"   "what about 2nd string"   )");
  libjson::Token t1 = lexer.next();
  libjson::Token t2 = lexer.next();
  EXPECT_EQ(t1.type, libjson::TokenTypes::STRING);
  EXPECT_EQ(t1.literal, "we've had one string yes");

  EXPECT_EQ(t2.type, libjson::TokenTypes::STRING);
  EXPECT_EQ(t2.literal, "what about 2nd string");
}

TEST(Lexer, eof) {
  libjson::Lexer lexer("");
  libjson::Token t = lexer.next();

  EXPECT_EQ(t.type, libjson::TokenTypes::END_OF_FILE);
  EXPECT_EQ(t.literal, "\0");
}
