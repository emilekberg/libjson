
#include <gtest/gtest.h>
#include <libjson/json-value.h>
#include <string>
using namespace libjson;

TEST(JsonValue, typeid_is_correct) {
  JsonValue value = {JsonValueType::STRING, std::string("some string")};
  EXPECT_EQ(value.value.type(), typeid(std::string));
}

TEST(JsonValue, bool) {
  JsonValue value = {JsonValueType::BOOL, true};
  EXPECT_FALSE(value.is(JsonValueType::STRING));
  EXPECT_TRUE(value.is(JsonValueType::BOOL));
}

TEST(JsonValue, is_type) {
  JsonValue value = {JsonValueType::STRING, std::string("some string")};
  EXPECT_TRUE(value.is(JsonValueType::STRING));
  EXPECT_FALSE(value.is(JsonValueType::BOOL));
}

TEST(JsonValue, get) {
  JsonValue value = {JsonValueType::STRING, std::string("some string")};
  EXPECT_EQ(value.get<std::string>(), "some string");
}

TEST(JsonValue, get_const_str) {
  const JsonValue value = {JsonValueType::STRING, std::string("some string")};
  EXPECT_EQ(value.get<std::string>(), "some string");
}

TEST(JsonValue, get_const) {
  const JsonValue value = {JsonValueType::BOOL, false};
  EXPECT_FALSE(value.get<bool>());
}

TEST(JsonValue, get_bool) {
  JsonValue value = {JsonValueType::BOOL, true};
  EXPECT_TRUE(value.get<bool>());
}

TEST(JsonValue, get_const_bool) {
  const JsonValue value = {JsonValueType::STRING, true};
  EXPECT_TRUE(value.get<bool>());
}
