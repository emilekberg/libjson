#include "libjson/json-types.h"
#include <gtest/gtest.h>
#include <libjson/parse.h>

TEST(parse, value_string) {
  std::string input = R"("someString")";
  libjson::LazyTokenizer tokens(input);
  libjson::Token token = tokens.next();
  libjson::JsonValue value = libjson::parseValue(token, tokens);

  EXPECT_EQ(value.getType(), libjson::ValueType::STRING);
  EXPECT_EQ(value.get<std::string>(), "someString");
}

TEST(parse, value_double) {
  std::string input = R"(1234.56)";
  libjson::LazyTokenizer tokens(input);
  libjson::Token token = tokens.next();
  libjson::JsonValue value = libjson::parseValue(token, tokens);

  EXPECT_EQ(value.getType(), libjson::ValueType::NUMBER);
  EXPECT_EQ(value.get<double>(), 1234.56);
}

TEST(parse, value_literal_true) {
  std::string input = R"(true)";
  libjson::LazyTokenizer tokens(input);
  libjson::Token token = tokens.next();
  libjson::JsonValue value = libjson::parseValue(token, tokens);

  EXPECT_EQ(value.getType(), libjson::ValueType::BOOL);
  EXPECT_TRUE(value.get<bool>());
}

TEST(parse, value_literal_false) {
  std::string input = R"(false)";
  libjson::LazyTokenizer tokens(input);
  libjson::Token token = tokens.next();
  libjson::JsonValue value = libjson::parseValue(token, tokens);

  EXPECT_EQ(value.getType(), libjson::ValueType::BOOL);
  EXPECT_FALSE(value.get<bool>());
}

TEST(parse, value_literal_null) {
  std::string input = R"(null)";
  libjson::LazyTokenizer tokens(input);
  libjson::Token token = tokens.next();
  libjson::JsonValue value = libjson::parseValue(token, tokens);

  EXPECT_EQ(value.getType(), libjson::ValueType::_NULL_);
  EXPECT_EQ(value.get<libjson::JsonNull>(), nullptr);
}

TEST(Parse, object_empty_object) {
  std::string input = R"({})";

  libjson::JsonValue value = libjson::parse(input);
  ASSERT_EQ(value.getType(), libjson::ValueType::OBJECT);

  libjson::JsonObject object = value.get<libjson::JsonObject>();
  ASSERT_EQ(object.size(), 0);
}

TEST(parse, array_empty_array) {
  std::string input = R"([])";
  libjson::JsonValue value = libjson::parse(input);
  ASSERT_EQ(value.getType(), libjson::ValueType::ARRAY);

  libjson::JsonArray array = value.get<libjson::JsonArray>();
  ASSERT_EQ(array.size(), 0);
}
