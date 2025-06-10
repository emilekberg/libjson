#include "libjson/json-value.h"
#include <gtest/gtest.h>
#include <libjson/json-object.h>

using namespace libjson;
TEST(JSONObject, has_returns_false) {
  JSONObject obj;
  EXPECT_FALSE(obj.has("key"));
}

TEST(JSONObject, can_add_values) {
  JSONObject obj;
  obj.add("key", {JSONValueType::STRING, std::string("value")});
  EXPECT_TRUE(obj.has("key"));
}

TEST(JSONObject, can_add_T_string) {
  JSONObject obj;
  const std::string constString = "anotherString";
  obj.add("key", std::string("value"));
  obj.add("keyForConst", constString);
  EXPECT_TRUE(obj.has("key"));
  EXPECT_EQ(obj.getValue("key").getType(), JSONValueType::STRING);
  EXPECT_TRUE(obj.has("keyForConst"));
  EXPECT_EQ(obj.getValue("keyForConst").getType(), JSONValueType::STRING);
}

TEST(JSONObject, can_add_T_bool) {
  JSONObject obj;
  obj.add("bool", true);
  EXPECT_TRUE(obj.has("bool"));
  EXPECT_EQ(obj.getValue("bool").getType(), JSONValueType::BOOL);
  EXPECT_TRUE(obj.get<bool>("bool"));
}

TEST(JSONObject, can_add_T_number) {
  JSONObject obj;
  obj.add("number", 1337);
  EXPECT_TRUE(obj.has("number"));
  EXPECT_EQ(obj.getValue("number").getType(), JSONValueType::NUMBER);
}

TEST(JSONObject, const_cast_has) {
  JSONObject obj;
  obj.add("key", {JSONValueType::STRING, std::string("value")});
  const JSONObject constObj = obj;
  EXPECT_TRUE(constObj.has("key"));
}

TEST(JSONObject, const_cast_get) {
  JSONObject obj;
  obj.add("key", {JSONValueType::STRING, std::string("value")});
  const JSONObject constObj = obj;
  EXPECT_EQ(constObj.get<std::string>("key"), "value");
}

TEST(JSONObject, const_number_test) {
  JSONObject obj;
  obj.add("number", 1337);
  EXPECT_TRUE(obj.has("number"));
  const JSONObject constObj = obj;
  EXPECT_EQ(constObj.get<int>("number"), 1337);
}
