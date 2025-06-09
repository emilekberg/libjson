
#include <gtest/gtest.h>
#include <libjson/json-value.h>
#include <string>
using namespace libjson;

TEST(JSONValue, typeid_is_correct) {
  JSONValue value = {JSONValueType::STRING, std::string("some string")};
  EXPECT_EQ(value.value.type(), typeid(std::string));
}

TEST(JSONValue, bool) {
  JSONValue value = {JSONValueType::BOOL, true};
  EXPECT_FALSE(value.is(JSONValueType::STRING));
  EXPECT_TRUE(value.is(JSONValueType::BOOL));
}

TEST(JSONValue, is_type) {
  JSONValue value = {JSONValueType::STRING, std::string("some string")};
  EXPECT_TRUE(value.is(JSONValueType::STRING));
  EXPECT_FALSE(value.is(JSONValueType::BOOL));
}

TEST(JSONValue, get) {
  JSONValue value = {JSONValueType::STRING, std::string("some string")};
  EXPECT_EQ(value.get<std::string>(), "some string");
}

TEST(JSONValue, get_const_str) {
  const JSONValue value = {JSONValueType::STRING, std::string("some string")};
  EXPECT_EQ(value.get<std::string>(), "some string");
}

TEST(JSONValue, get_const) {
  const JSONValue value = {JSONValueType::BOOL, false};
  EXPECT_FALSE(value.get<bool>());
}

TEST(JSONValue, get_bool) {
  JSONValue value = {JSONValueType::BOOL, true};
  EXPECT_TRUE(value.get<bool>());
}

TEST(JSONValue, get_const_bool) {
  const JSONValue value = {JSONValueType::STRING, true};
  EXPECT_TRUE(value.get<bool>());
}
