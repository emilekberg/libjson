#include "libjson/json-value.h"
#include <gtest/gtest.h>
#include <libjson/parse.h>

TEST(parse, value_string) {
  libjson::Tokenizer tokens = libjson::tokenize(R"("someString")");
  // we know first token is an array, so discard it.
  libjson::Token token = tokens.next();
  libjson::JSONValue value = libjson::parseValue(token, tokens);

  EXPECT_EQ(value.type, libjson::JSONValueType::STRING);
  EXPECT_EQ(value.get<std::string>(), "someString");
}

TEST(parse, value_double) {
  libjson::Tokenizer tokens = libjson::tokenize(R"(1234.56)");
  // we know first token is an array, so discard it.
  libjson::Token token = tokens.next();
  libjson::JSONValue value = libjson::parseValue(token, tokens);

  EXPECT_EQ(value.type, libjson::JSONValueType::NUMBER);
  EXPECT_EQ(value.get<double>(), 1234.56);
}

TEST(parse, value_literal_true) {
  libjson::Tokenizer tokens = libjson::tokenize(R"(true)");
  // we know first token is an array, so discard it.
  libjson::Token token = tokens.next();
  libjson::JSONValue value = libjson::parseValue(token, tokens);

  EXPECT_EQ(value.type, libjson::JSONValueType::BOOL);
  EXPECT_TRUE(value.get<bool>());
}

TEST(parse, value_literal_false) {
  libjson::Tokenizer tokens = libjson::tokenize(R"(false)");
  // we know first token is an array, so discard it.
  libjson::Token token = tokens.next();
  libjson::JSONValue value = libjson::parseValue(token, tokens);

  EXPECT_EQ(value.type, libjson::JSONValueType::BOOL);
  EXPECT_FALSE(value.get<bool>());
}

TEST(parse, value_literal_null) {
  libjson::Tokenizer tokens = libjson::tokenize(R"(null)");
  // we know first token is an array, so discard it.
  libjson::Token token = tokens.next();
  libjson::JSONValue value = libjson::parseValue(token, tokens);

  EXPECT_EQ(value.type, libjson::JSONValueType::_NULL);
  // EXPECT_EQ(value.get<int>(), nullptr);
}
