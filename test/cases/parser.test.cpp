#include "libjson/json-array.h"
#include "libjson/json-object.h"
#include "libjson/json-value.h"
#include <gtest/gtest.h>
#include <libjson/parse.h>

TEST(Parser, parses_object_one_key) {
  libjson::JSONValue val = libjson::parse(R"({"key":"value"})");

  libjson::JSONObject result = val.get<libjson::JSONObject>();
  ASSERT_TRUE(result.has("key"));

  libjson::JSONValue value = result.getValue("key");
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

  libjson::JSONValue valueA = result.getValue("a");
  EXPECT_EQ(valueA.getType(), libjson::JSONValueType::STRING);
  EXPECT_EQ(std::any_cast<std::string>(valueA.value), "first");
  EXPECT_EQ(valueA.get<std::string>(), "first");

  libjson::JSONValue valueB = result.getValue("b");
  EXPECT_EQ(valueB.getType(), libjson::JSONValueType::STRING);
  EXPECT_EQ(std::any_cast<std::string>(valueB.value), "second");
  EXPECT_EQ(valueB.get<std::string>(), "second");
  EXPECT_EQ(result.get<std::string>("b"), "second");
}

TEST(Parser, parses_object_with_object) {
  libjson::JSONValue val =
      libjson::parse(R"({"root":{"branch":"deep value"}})");

  libjson::JSONObject result = val.get<libjson::JSONObject>();
  ASSERT_TRUE(result.has("root"));

  libjson::JSONValue rootValue = result.getValue("root");
  auto root = rootValue.get<libjson::JSONObject>();
  ASSERT_TRUE(root.has("branch"));

  libjson::JSONValue branchValue = root.getValue("branch");
  std::string branchstr = branchValue.get<std::string>();
  EXPECT_EQ(branchValue.getType(), libjson::JSONValueType::STRING);
  EXPECT_EQ(branchstr, "deep value");
}

TEST(Parser, parses_array_with_values) {
  std::string input = R"([{"a":1},{"a":2},{"a":3}])";
  std::vector<double> expected_arr = {1, 2, 3};
  libjson::JSONValue value = libjson::parse(input);

  ASSERT_EQ(value.getType(), libjson::JSONValueType::ARRAY);
  libjson::JSONArray array = value.get<libjson::JSONArray>();
  ASSERT_EQ(array.size(), expected_arr.size());

  for (size_t i = 0; i < array.size(); i++) {
    int expected = expected_arr[i];
    int actual = array[i].get<libjson::JSONObject>().get<int>("a");
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
  libjson::JSONValue value = libjson::parse(json);
  EXPECT_EQ(value.getType(), libjson::JSONValueType::OBJECT);
}
