#include <gtest/gtest.h>
#include <libjson/parse.h>

TEST(Parser, parses_object_one_key) {
  libjson::JSONValue val = libjson::parse(R"({"key":"value"})");

  libjson::JSONObject result = val.get<libjson::JSONObject>();
  ASSERT_TRUE(result.has("key"));

  libjson::JSONValue value = result.get("key");
  EXPECT_EQ(value.type, libjson::JSONValueType::STRING);
  EXPECT_EQ(std::any_cast<std::string>(value.value), "value");
  EXPECT_EQ(value.get<std::string>(), "value");
  // EXPECT_EQ(result.get<std::string>("key"), "value");
}

TEST(Parser, parses_object_two_keys) {
  libjson::JSONValue val = libjson::parse(R"({"a":"first","b":"second"})");

  libjson::JSONObject result = val.get<libjson::JSONObject>();
  ASSERT_TRUE(result.has("a"));
  ASSERT_TRUE(result.has("b"));

  libjson::JSONValue valueA = result.get("a");
  EXPECT_EQ(valueA.type, libjson::JSONValueType::STRING);
  EXPECT_EQ(std::any_cast<std::string>(valueA.value), "first");
  EXPECT_EQ(valueA.get<std::string>(), "first");

  libjson::JSONValue valueB = result.get("b");
  EXPECT_EQ(valueB.type, libjson::JSONValueType::STRING);
  EXPECT_EQ(std::any_cast<std::string>(valueB.value), "second");
  EXPECT_EQ(valueB.get<std::string>(), "second");
  // EXPECT_EQ(result.get<std::string>("key"), "value");
}

TEST(Parser, parses_object_with_object) {
  libjson::JSONValue val =
      libjson::parse(R"({"root":{"branch":"deep value"}})");

  libjson::JSONObject result = val.get<libjson::JSONObject>();
  ASSERT_TRUE(result.has("root"));

  libjson::JSONValue rootValue = result.get("root");
  auto root = rootValue.get<libjson::JSONObject>();
  ASSERT_TRUE(root.has("branch"));

  libjson::JSONValue branchValue = root.get("branch");
  EXPECT_EQ(branchValue.type, libjson::JSONValueType::STRING);
  EXPECT_EQ(branchValue.get<std::string>(), "deep value");
}
