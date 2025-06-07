#include <gtest/gtest.h>
#include <libjson/object.h>
#include <libjson/parse.h>

TEST(Parser, parses_simple_object) {
  libjson::Object result = libjson::parse(R"({"key":"value"})");
  ASSERT_TRUE(result.has("key"));

  libjson::Value value = result.get("key");
  EXPECT_EQ(value.type, libjson::ValueType::STRING);
  EXPECT_EQ(std::any_cast<std::string>(value.value), "value");
  EXPECT_EQ(value.get<std::string>(), "value");
  // EXPECT_EQ(result.get<std::string>("key"), "value");
}
