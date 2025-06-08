#include "libjson/json-value.h"
#include <gtest/gtest.h>
#include <libjson/parse.h>

TEST(parse, value_string) {
  std::string input = R"("someString")";
  libjson::Tokenizer tokens = libjson::tokenize(input);
  libjson::Token token = tokens.next();
  libjson::JSONValue value = libjson::parseValue(token, tokens);

  EXPECT_EQ(value.type, libjson::JSONValueType::STRING);
  EXPECT_EQ(value.get<std::string>(), "someString");
}

TEST(parse, value_double) {
  std::string input = R"(1234.56)";
  libjson::Tokenizer tokens = libjson::tokenize(input);
  libjson::Token token = tokens.next();
  libjson::JSONValue value = libjson::parseValue(token, tokens);

  EXPECT_EQ(value.type, libjson::JSONValueType::NUMBER);
  EXPECT_EQ(value.get<double>(), 1234.56);
}

TEST(parse, value_literal_true) {
  std::string input = R"(true)";
  libjson::Tokenizer tokens = libjson::tokenize(input);
  libjson::Token token = tokens.next();
  libjson::JSONValue value = libjson::parseValue(token, tokens);

  EXPECT_EQ(value.type, libjson::JSONValueType::BOOL);
  EXPECT_TRUE(value.get<bool>());
}

TEST(parse, value_literal_false) {
  std::string input = R"(false)";
  libjson::Tokenizer tokens = libjson::tokenize(input);
  libjson::Token token = tokens.next();
  libjson::JSONValue value = libjson::parseValue(token, tokens);

  EXPECT_EQ(value.type, libjson::JSONValueType::BOOL);
  EXPECT_FALSE(value.get<bool>());
}

TEST(parse, value_literal_null) {
  std::string input = R"(null)";
  libjson::Tokenizer tokens = libjson::tokenize(input);
  libjson::Token token = tokens.next();
  libjson::JSONValue value = libjson::parseValue(token, tokens);

  EXPECT_EQ(value.type, libjson::JSONValueType::_NULL);
  // EXPECT_EQ(value.get<int>(), nullptr);
}
