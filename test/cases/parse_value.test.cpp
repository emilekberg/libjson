#include "libjson/json-object.h"
#include "libjson/json-value.h"
#include <gtest/gtest.h>
#include <libjson/parse.h>

TEST(parse, value_string) {
  std::string input = R"("someString")";
  libjson::LazyTokenizer tokens(input);
  libjson::Token token = tokens.next();
  libjson::JsonValue value = libjson::parseValue(token, tokens);

  EXPECT_EQ(value.getType(), libjson::JsonValueType::STRING);
  EXPECT_EQ(value.get<std::string>(), "someString");
}

TEST(parse, value_double) {
  std::string input = R"(1234.56)";
  libjson::LazyTokenizer tokens(input);
  libjson::Token token = tokens.next();
  libjson::JsonValue value = libjson::parseValue(token, tokens);

  EXPECT_EQ(value.getType(), libjson::JsonValueType::NUMBER);
  EXPECT_EQ(value.get<double>(), 1234.56);
}

TEST(parse, value_literal_true) {
  std::string input = R"(true)";
  libjson::LazyTokenizer tokens(input);
  libjson::Token token = tokens.next();
  libjson::JsonValue value = libjson::parseValue(token, tokens);

  EXPECT_EQ(value.getType(), libjson::JsonValueType::BOOL);
  EXPECT_TRUE(value.get<bool>());
}

TEST(parse, value_literal_false) {
  std::string input = R"(false)";
  libjson::LazyTokenizer tokens(input);
  libjson::Token token = tokens.next();
  libjson::JsonValue value = libjson::parseValue(token, tokens);

  EXPECT_EQ(value.getType(), libjson::JsonValueType::BOOL);
  EXPECT_FALSE(value.get<bool>());
}

TEST(parse, value_literal_null) {
  std::string input = R"(null)";
  libjson::LazyTokenizer tokens(input);
  libjson::Token token = tokens.next();
  libjson::JsonValue value = libjson::parseValue(token, tokens);

  EXPECT_EQ(value.getType(), libjson::JsonValueType::_NULL);
  // EXPECT_EQ(value.get<int>(), nullptr);
}

TEST(Parse, object_empty_object) {
  std::string input = R"({})";

  libjson::JsonValue value = libjson::parse(input);
  ASSERT_EQ(value.getType(), libjson::JsonValueType::OBJECT);

  libjson::JsonObject object = value.get<libjson::JsonObject>();
  ASSERT_EQ(object.keys().size(), 0);
}

TEST(parse, array_empty_array) {
  std::string input = R"([])";
  libjson::JsonValue value = libjson::parse(input);
  ASSERT_EQ(value.getType(), libjson::JsonValueType::ARRAY);

  libjson::JsonArray array = value.get<libjson::JsonArray>();
  ASSERT_EQ(array.size(), 0);
}
