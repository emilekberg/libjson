#include "libjson/token_types.h"
#include <gtest/gtest.h>
#include <libjson/lexer.h>

using namespace libjson;

TEST(Lexer, skips_leading_whitespace) {
  std::istringstream input(R"(       "ignored leading whitespace"      )");
  Lexer lexer(input);
  libjson::Token t = lexer.next();
  EXPECT_EQ(t.type, libjson::TokenTypes::STRING);
  EXPECT_EQ(t.literal, "ignored leading whitespace");
}

TEST(Lexer, skips_middle_whitespace) {
  std::istringstream input(
      R"(       "we've had one string yes"   "what about 2nd string"   )");

  Lexer lexer(input);
  libjson::Token t1 = lexer.next();
  libjson::Token t2 = lexer.next();
  EXPECT_EQ(t1.type, libjson::TokenTypes::STRING);
  EXPECT_EQ(t1.literal, "we've had one string yes");

  EXPECT_EQ(t2.type, libjson::TokenTypes::STRING);
  EXPECT_EQ(t2.literal, "what about 2nd string");
}

TEST(Lexer, eof) {
  std::istringstream input("");

  Lexer lexer(input);
  libjson::Token t = lexer.next();

  EXPECT_EQ(t.type, libjson::TokenTypes::END_OF_FILE);
  EXPECT_EQ(t.literal, "\0");
}

TEST(Lexer, string_escape_edgecase) {
  std::istringstream input(R"({
  "message":"simpler non-flash version\\",
  "distinct":true
})");

  std::vector<libjson::Token> expected_tokens = {
      {TokenTypes::LEFT_BRACE},
      {TokenTypes::STRING, "message"},
      {TokenTypes::COLON},
      {TokenTypes::STRING, "simpler non-flash version\\\\"},
      {TokenTypes::COMMA},
      {TokenTypes::STRING, "distinct"},
      {TokenTypes::COLON},
      {TokenTypes::LITERAL, "true"},
      {TokenTypes::RIGHT_BRACE},
      {TokenTypes::END_OF_FILE, "\0"}};
  std::vector<libjson::Token> actual_tokens;

  Lexer lexer(input);
  while (true) {
    Token token = lexer.next();
    ASSERT_NE(token.type, TokenTypes::ILLEGAL);
    actual_tokens.push_back(token);
    if (token.type == TokenTypes::END_OF_FILE) {
      break;
    }
  }

  EXPECT_EQ(actual_tokens.size(), expected_tokens.size());
  for (size_t i = 0; i < actual_tokens.size(); i++) {
    const auto &actual = actual_tokens[i];
    const auto &expected = expected_tokens[i];

    EXPECT_EQ(actual.type, expected.type);
    EXPECT_EQ(actual.literal, expected.literal);
  }
}

TEST(Lexer, trailing_comma) {
  std::istringstream input(R"({
    "public": true,
    "created_at": "2015-01-01T15:00:00Z",
})");

  std::vector<libjson::Token> expected_tokens = {
      {TokenTypes::LEFT_BRACE},
      {TokenTypes::STRING, "public"},
      {TokenTypes::COLON},
      {TokenTypes::LITERAL, "true"},
      {TokenTypes::COMMA},
      {TokenTypes::STRING, "created_at"},
      {TokenTypes::COLON},
      {TokenTypes::STRING, "2015-01-01T15:00:00Z"},
      {TokenTypes::COMMA},
      {TokenTypes::RIGHT_BRACE},
      {TokenTypes::END_OF_FILE, "\0"}};

  std::vector<libjson::Token> actual_tokens;
  Lexer lexer(input);
  while (true) {
    Token token = lexer.next();
    ASSERT_NE(token.type, TokenTypes::ILLEGAL);
    actual_tokens.push_back(token);
    if (token.type == TokenTypes::END_OF_FILE) {
      break;
    }
  }

  EXPECT_EQ(actual_tokens.size(), expected_tokens.size());
  for (size_t i = 0; i < actual_tokens.size(); i++) {
    const auto &actual = actual_tokens[i];
    const auto &expected = expected_tokens[i];

    EXPECT_EQ(actual.type, expected.type);
    EXPECT_EQ(actual.literal, expected.literal);
  }
}

TEST(Lexer, iterator) {
  std::istringstream input(R"({
    "public": true,
    "created_at": "2015-01-01T15:00:00Z",
})");

  std::vector<libjson::Token> expected_tokens = {
      {TokenTypes::LEFT_BRACE},
      {TokenTypes::STRING, "public"},
      {TokenTypes::COLON},
      {TokenTypes::LITERAL, "true"},
      {TokenTypes::COMMA},
      {TokenTypes::STRING, "created_at"},
      {TokenTypes::COLON},
      {TokenTypes::STRING, "2015-01-01T15:00:00Z"},
      {TokenTypes::COMMA},
      {TokenTypes::RIGHT_BRACE},
      {TokenTypes::END_OF_FILE, "\0"}};

  size_t count = 0;
  for (auto actual : Lexer(input)) {
    Token expected = expected_tokens[count++];
    EXPECT_EQ(actual.type, expected.type);
    EXPECT_EQ(actual.literal, expected.literal);
  }
  ASSERT_EQ(count, expected_tokens.size());
}
