#include "libjson/json-types.h"
#include "libjson/parse.h"
#include <gtest/gtest.h>
#include <sstream>

using namespace libjson;
TEST(JsonObject, has_returns_false) {
   JsonObject obj;
   EXPECT_FALSE(obj.has("key"));
}

TEST(JsonObject, can_add_values) {
   std::istringstream stream(R"({"key":"value"})");
   JsonValue          json = libjson::parse(stream);
   JsonObject        &obj  = json.get<JsonObject>();
   EXPECT_TRUE(obj.has("key"));
}

TEST(JsonObject, can_add_T_string) {
   std::istringstream stream(R"({"key":"value","keyForConst": "value2"})");
   JsonValue          json = libjson::parse(stream);
   JsonObject        &obj  = json.get<JsonObject>();
   EXPECT_TRUE(obj.has("key"));
   EXPECT_EQ(obj.get<JsonValue>("key").getType(), ValueType::STRING);
   EXPECT_TRUE(obj.has("keyForConst"));
   EXPECT_EQ(obj.get<JsonValue>("keyForConst").getType(), ValueType::STRING);
}

TEST(JsonObject, can_add_T_bool) {
   std::istringstream stream(R"({"bool":false})");
   JsonValue          json = libjson::parse(stream);
   JsonObject        &obj  = json.get<JsonObject>();
   EXPECT_TRUE(obj.has("bool"));
   EXPECT_EQ(obj.get<JsonValue>("bool").getType(), ValueType::BOOL);
   EXPECT_FALSE(obj.get<bool>("bool"));
}

TEST(JsonObject, can_add_T_number) {
   std::istringstream stream(R"({"number":1337})");
   JsonValue          json = libjson::parse(stream);
   JsonObject        &obj  = json.get<JsonObject>();
   EXPECT_TRUE(obj.has("number"));
   EXPECT_EQ(obj.get<JsonValue>("number").getType(), ValueType::NUMBER);
   EXPECT_EQ(obj.get<int>("number"), 1337);
}

TEST(JsonObject, const_cast_has) {
   std::istringstream stream(R"({"key":"value"})");
   JsonValue          json     = libjson::parse(stream);
   const JsonObject  &constObj = json.get<JsonObject>();
   EXPECT_TRUE(constObj.has("key"));
   EXPECT_EQ(constObj.get<JsonValue>("key").getType(), ValueType::STRING);
}

TEST(JsonObject, const_cast_get) {
   std::istringstream stream(R"({"key":"value"})");
   JsonValue          json     = libjson::parse(stream);
   const JsonObject  &constObj = json.get<JsonObject>();
   EXPECT_EQ(constObj.get<JsonString>("key"), "value");
}

TEST(JsonObject, const_number_test) {
   std::istringstream stream(R"({"number":1337})");
   JsonValue          json = libjson::parse(stream);
   JsonObject        &obj  = json.get<JsonObject>();
   EXPECT_TRUE(obj.has("number"));
   EXPECT_EQ(obj.get<int>("number"), 1337);
}

TEST(JsonObject, iterator) {
   std::istringstream stream(R"({"a":1,"b":2,"c":3})");
   JsonValue          json = libjson::parse(stream);
   JsonObject        &obj  = json.get<JsonObject>();

   EXPECT_EQ(obj.size(), 3);
   for (auto &v : obj) {
      EXPECT_GT(v.second.get<int>(), 0);
   }
}
