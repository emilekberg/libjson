#include <gtest/gtest.h>
#include <libjson/lexer.h>
#include <vector>
using namespace libjson;
TEST(Lexer, object_with_key) {
  std::vector<libjson::Token> expected_tokens = {
      {TokenTypes::SEPARATOR, "{"}, {TokenTypes::STRING, "key"},
      {TokenTypes::SEPARATOR, ":"}, {TokenTypes::STRING, "value"},
      {TokenTypes::SEPARATOR, "}"},
  };
  libjson::Lexer lexer(R"({"key":"value"})");
  libjson::Token t;
  std::vector<libjson::Token> actual_tokens;
  while (true) {
    t = lexer.next();
    if (t.type == libjson::TokenTypes::END_OF_FILE) {
      break;
    }
    actual_tokens.push_back(t);
  }

  ASSERT_EQ(actual_tokens.size(), expected_tokens.size());
  for (size_t i = 0; i < actual_tokens.size(); i++) {
    const auto &actual = actual_tokens[i];
    const auto &expected = expected_tokens[i];

    EXPECT_EQ(actual.type, expected.type);
    EXPECT_EQ(actual.literal, expected.literal);
  }
}

TEST(Lexer, object_with_number_array) {
  std::vector<libjson::Token> expected_tokens = {
      {TokenTypes::SEPARATOR, "{"}, {TokenTypes::STRING, "numbers"},
      {TokenTypes::SEPARATOR, ":"}, {TokenTypes::SEPARATOR, "["},
      {TokenTypes::NUMBER, "1"},    {TokenTypes::SEPARATOR, ","},
      {TokenTypes::NUMBER, "2"},    {TokenTypes::SEPARATOR, ","},
      {TokenTypes::NUMBER, "3"},    {TokenTypes::SEPARATOR, ","},
      {TokenTypes::SEPARATOR, "]"}, {TokenTypes::SEPARATOR, "}"},
  };
  libjson::Lexer lexer(R"({"numbers":[1, 2, 3,]})");
  libjson::Token t;
  std::vector<libjson::Token> actual_tokens;
  while (true) {
    t = lexer.next();
    if (t.type == libjson::TokenTypes::END_OF_FILE) {
      break;
    }
    actual_tokens.push_back(t);
  }

  ASSERT_EQ(actual_tokens.size(), expected_tokens.size());
  for (size_t i = 0; i < actual_tokens.size(); i++) {
    const auto &actual = actual_tokens[i];
    const auto &expected = expected_tokens[i];

    EXPECT_EQ(actual.type, expected.type);
    EXPECT_EQ(actual.literal, expected.literal);
  }
}
