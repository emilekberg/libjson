#include <gtest/gtest.h>
#include <libjson/json-types.h>
#include <libjson/parse.h>

using namespace libjson;
TEST(Parser, parses_object_one_key) {
  libjson::JsonValue val = libjson::parse(R"({"key":"value"})");

  libjson::JsonObject result = val.get<libjson::JsonObject>();
  ASSERT_TRUE(result.has("key"));

  JsonValue value = result.get<JsonValue>("key");
  EXPECT_EQ(value.getType(), libjson::ValueType::STRING);
  EXPECT_EQ(value.get<std::string>(), "value");
  EXPECT_EQ(result.get<JsonString>("key"), "value");
}

TEST(Parser, parses_object_two_keys) {
  JsonValue val = libjson::parse(R"({"a":"first","b":"second"})");

  JsonObject result = val.get<libjson::JsonObject>();
  ASSERT_TRUE(result.has("a"));
  ASSERT_TRUE(result.has("b"));

  JsonValue valueA = result.get<JsonValue>("a");
  EXPECT_EQ(valueA.getType(), ValueType::STRING);
  EXPECT_EQ(result.get<std::string>("a"), "first");

  libjson::JsonValue valueB = result.get<JsonValue>("b");
  EXPECT_EQ(valueB.getType(), ValueType::STRING);
  EXPECT_EQ(result.get<std::string>("b"), "second");
  EXPECT_EQ(result.get<std::string>("b"), "second");
}

TEST(Parser, parses_object_with_object) {
  libjson::JsonValue val =
      libjson::parse(R"({"root":{"branch":"deep value"}})");

  libjson::JsonObject result = val.get<libjson::JsonObject>();
  ASSERT_TRUE(result.has("root"));

  libjson::JsonValue rootValue = result.get<JsonValue>("root");
  auto root = rootValue.get<libjson::JsonObject>();
  ASSERT_TRUE(root.has("branch"));

  libjson::JsonValue branchValue = root.get<JsonValue>("branch");
  std::string branchstr = root.get<JsonString>("branch");
  EXPECT_EQ(branchValue.getType(), ValueType::STRING);
  EXPECT_EQ(branchstr, "deep value");
}

TEST(Parser, parses_array_with_values) {
  std::string input = R"([{"a":1},{"a":2},{"a":3}])";
  std::vector<double> expected_arr = {1, 2, 3};
  libjson::JsonValue value = libjson::parse(input);

  ASSERT_EQ(value.getType(), libjson::ValueType::ARRAY);
  libjson::JsonArray array = value.get<libjson::JsonArray>();
  ASSERT_EQ(array.size(), expected_arr.size());

  for (size_t i = 0; i < array.size(); i++) {
    int expected = expected_arr[i];
    int actual = array.get<JsonObject>(i).get<int>("a");
    EXPECT_EQ(actual, expected);
  }
}

TEST(Parser, objects_with_trailing_comma) {
  std::string json = R"({
    "id": "2489651045",
    "actor": {
        "id": 665991,
    },
    "repo": {
        "id": 28688495,
        "name": "petroav/6.828",
        "url": "https://api.github.com/repos/petroav/6.828",
    },
    "payload": {
        "ref": "master",
        "ref_type": "branch",
        "master_branch": "master",
        "description":
            "Solution to homework and assignments from MIT's 6.828 (Operating Systems Engineering). Done in my spare time.",
        "pusher_type": "user",
    },
    "public": true,
    "created_at": "2015-01-01T15:00:00Z",
})";
  JsonValue value = parse(json);
  EXPECT_EQ(value.getType(), ValueType::OBJECT);
}
