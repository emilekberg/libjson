#include <gtest/gtest.h>
#include <libjson/lexer.h>
#include <vector>
using namespace libjson;
TEST(Lexer, object_with_key) {
  std::vector<libjson::Token> expected_tokens = {
      {TokenTypes::CURLY_BRACE_OPEN},
      {TokenTypes::STRING, "key"},
      {TokenTypes::COLON},
      {TokenTypes::STRING, "value"},
      {TokenTypes::CURLY_BRACE_CLOSE},
  };
  std::istringstream input(R"({"key":"value"})");
  libjson::Lexer lexer(input);
  std::vector<libjson::Token> actual_tokens;
  while (true) {
    libjson::Token t = lexer.next();
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
      {TokenTypes::CURLY_BRACE_OPEN},
      {TokenTypes::STRING, "numbers"},
      {TokenTypes::COLON},
      {TokenTypes::SQUARE_BRACKET_OPEN},
      {TokenTypes::NUMBER, "1"},
      {TokenTypes::COMMA},
      {TokenTypes::NUMBER, "2"},
      {TokenTypes::COMMA},
      {TokenTypes::NUMBER, "3"},
      {TokenTypes::COMMA},
      {TokenTypes::SQUARE_BRACKET_CLOSE},
      {TokenTypes::CURLY_BRACE_CLOSE},
  };
  std::istringstream input(R"({"numbers":[1, 2, 3,]})");
  libjson::Lexer lexer(input);
  std::vector<libjson::Token> actual_tokens;
  while (true) {
    libjson::Token t = lexer.next();
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
