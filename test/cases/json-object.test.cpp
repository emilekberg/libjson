#include "libjson/json-value-types.h"
#include "libjson/json-value.h"
#include <gtest/gtest.h>
#include <libjson/json-object.h>

using namespace libjson;
TEST(JsonObject, has_returns_false) {
  JsonObject obj;
  EXPECT_FALSE(obj.has("key"));
}

TEST(JsonObject, can_add_values) {
  JsonObject obj({{"key", {JsonValueType::STRING, std::string("value")}}});
  EXPECT_TRUE(obj.has("key"));
}

TEST(JsonObject, can_add_T_string) {
  const std::string constString = "anotherString";
  JsonObject obj({{"key", {JsonValueType::STRING, std::string("value")}},
                  {"keyForConst", {JsonValueType::STRING, constString}}});
  EXPECT_TRUE(obj.has("key"));
  EXPECT_EQ(obj.getValue("key").getType(), JsonValueType::STRING);
  EXPECT_TRUE(obj.has("keyForConst"));
  EXPECT_EQ(obj.getValue("keyForConst").getType(), JsonValueType::STRING);
}

TEST(JsonObject, can_add_T_bool) {
  JsonObject obj({{"bool", {JsonValueType::BOOL, false}}});
  EXPECT_TRUE(obj.has("bool"));
  EXPECT_EQ(obj.getValue("bool").getType(), JsonValueType::BOOL);
  EXPECT_FALSE(obj.get<bool>("bool"));
}

TEST(JsonObject, can_add_T_number) {
  JsonObject obj({{"number", {JsonValueType::NUMBER, 1337}}});
  EXPECT_TRUE(obj.has("number"));
  EXPECT_EQ(obj.getValue("number").getType(), JsonValueType::NUMBER);
}

TEST(JsonObject, const_cast_has) {
  JsonObject obj({{"key", {JsonValueType::STRING, std::string("value")}}});
  const JsonObject constObj = obj;
  EXPECT_TRUE(constObj.has("key"));
}

TEST(JsonObject, const_cast_get) {
  JsonObject obj({{"key", {JsonValueType::STRING, std::string("value")}}});
  const JsonObject constObj = obj;
  EXPECT_EQ(constObj.get<std::string>("key"), "value");
}

// TEST(JsonObject, const_number_test) {
//   JsonObject obj({{"number", {JsonValueType::NUMBER, 1337}}});
//   EXPECT_TRUE(obj.has("number"));
//   const JsonObject constObj = obj;
//   EXPECT_EQ(constObj.get<int>("number"), 1337);
// }
