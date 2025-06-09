#include "libjson/json-object.h"
#include "libjson/json-value.h"
#include <gtest/gtest.h>
#include <libjson/parse.h>

TEST(parse, value_string) {
  std::string input = R"("someString")";
  libjson::LazyTokenizer tokens(input);
  libjson::Token token = tokens.next();
  libjson::JSONValue value = libjson::parseValue(token, tokens);

  EXPECT_EQ(value.getType(), libjson::JSONValueType::STRING);
  EXPECT_EQ(value.get<std::string>(), "someString");
}

TEST(parse, value_double) {
  std::string input = R"(1234.56)";
  libjson::LazyTokenizer tokens(input);
  libjson::Token token = tokens.next();
  libjson::JSONValue value = libjson::parseValue(token, tokens);

  EXPECT_EQ(value.getType(), libjson::JSONValueType::NUMBER);
  EXPECT_EQ(value.get<double>(), 1234.56);
}

TEST(parse, value_literal_true) {
  std::string input = R"(true)";
  libjson::LazyTokenizer tokens(input);
  libjson::Token token = tokens.next();
  libjson::JSONValue value = libjson::parseValue(token, tokens);

  EXPECT_EQ(value.getType(), libjson::JSONValueType::BOOL);
  EXPECT_TRUE(value.get<bool>());
}

TEST(parse, value_literal_false) {
  std::string input = R"(false)";
  libjson::LazyTokenizer tokens(input);
  libjson::Token token = tokens.next();
  libjson::JSONValue value = libjson::parseValue(token, tokens);

  EXPECT_EQ(value.getType(), libjson::JSONValueType::BOOL);
  EXPECT_FALSE(value.get<bool>());
}

TEST(parse, value_literal_null) {
  std::string input = R"(null)";
  libjson::LazyTokenizer tokens(input);
  libjson::Token token = tokens.next();
  libjson::JSONValue value = libjson::parseValue(token, tokens);

  EXPECT_EQ(value.getType(), libjson::JSONValueType::_NULL);
  // EXPECT_EQ(value.get<int>(), nullptr);
}

TEST(Parse, object_empty_object) {
  std::string input = R"({})";

  libjson::JSONValue value = libjson::parse(input);
  ASSERT_EQ(value.getType(), libjson::JSONValueType::OBJECT);

  libjson::JSONObject object = value.get<libjson::JSONObject>();
  ASSERT_EQ(object.keys().size(), 0);
}

TEST(parse, array_empty_array) {
  std::string input = R"([])";
  libjson::JSONValue value = libjson::parse(input);
  ASSERT_EQ(value.getType(), libjson::JSONValueType::ARRAY);

  libjson::JSONArray array = value.get<libjson::JSONArray>();
  ASSERT_EQ(array.size(), 0);
}
