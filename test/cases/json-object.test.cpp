#include "libjson/json-types.h"
#include <gtest/gtest.h>

using namespace libjson;
TEST(JsonObject, has_returns_false) {
   JsonObject obj;
   EXPECT_FALSE(obj.has("key"));
}

TEST(JsonObject, can_add_values) {
   JsonObject obj({{"key", "value"}});
   EXPECT_TRUE(obj.has("key"));
}

TEST(JsonObject, can_add_T_string) {
   const std::string constString = "anotherString";
   JsonObject        obj({{"key", "value"}, {"keyForConst", constString}});
   EXPECT_TRUE(obj.has("key"));
   EXPECT_EQ(obj.get<JsonValue>("key").getType(), ValueType::STRING);
   EXPECT_TRUE(obj.has("keyForConst"));
   EXPECT_EQ(obj.get<JsonValue>("keyForConst").getType(), ValueType::STRING);
}

TEST(JsonObject, can_add_T_bool) {
   JsonObject obj({{"bool", false}});
   EXPECT_TRUE(obj.has("bool"));
   EXPECT_EQ(obj.get<JsonValue>("bool").getType(), ValueType::BOOL);
   EXPECT_FALSE(obj.get<bool>("bool"));
}

TEST(JsonObject, can_add_T_number) {
   JsonObject obj({{"number", JsonNumber("1337")}});
   EXPECT_TRUE(obj.has("number"));
   EXPECT_EQ(obj.get<JsonValue>("number").getType(), ValueType::NUMBER);
   EXPECT_EQ(obj.get<int>("number"), 1337);
}

TEST(JsonObject, const_cast_has) {
   JsonObject       obj({{"key", "value"}});
   const JsonObject constObj = obj;
   EXPECT_TRUE(constObj.has("key"));
   EXPECT_EQ(constObj.get<JsonValue>("key").getType(), ValueType::STRING);
}

TEST(JsonObject, const_cast_get) {
   JsonObject       obj({{"key", "value"}});
   const JsonObject constObj = obj;
   EXPECT_EQ(constObj.get<JsonString>("key"), "value");
}

TEST(JsonObject, const_number_test) {
   JsonObject obj({{"number", JsonNumber("1337")}});
   EXPECT_TRUE(obj.has("number"));
   const JsonObject constObj = obj;
   EXPECT_EQ(constObj.get<int>("number"), 1337);
}

TEST(JsonObject, iterator) {
   JsonObject obj({{"a", JsonNumber("1")}, {"b", JsonNumber("2")}, {"c", JsonNumber("3")}});

   EXPECT_EQ(obj.size(), 3);
   size_t i = 0;
   for (auto v : obj) {
      EXPECT_GT(v.second.get<int>(), 0);
   }
}
