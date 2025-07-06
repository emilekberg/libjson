#include "gtest/gtest.h"
#include <format>
#include <fstream>
#include <gtest/gtest.h>
#include <libjson/exceptions.h>
#include <libjson/parse.h>

class JsonCheckerPassesTests : public ::testing::TestWithParam<std::string> {
 protected:
   std::string path;

   void SetUp() override {
      path = std::format("data/json-checker/{}", GetParam());
   }
};

class JsonCheckerFailsTests : public JsonCheckerPassesTests {};

TEST_P(JsonCheckerFailsTests, fails) {
   std::ifstream file(path);
   ASSERT_TRUE(file.is_open());
   EXPECT_THROW(libjson::parse(file), libjson::unexpected_token);
}
INSTANTIATE_TEST_SUITE_P(
    JsonCheckerTestSuite,
    JsonCheckerFailsTests,
    ::testing::Values(
        "fail1.json",
        "fail2.json",
        "fail3.json",
        // "fail4.json", // NOTE: trailing comma
        // "fail5.json", // NOTE: trailing comma.
        "fail6.json",
        "fail7.json",
        "fail8.json",

        // "fail9.json", // trailing comma
        "fail10.json",
        "fail11.json",
        "fail12.json",
        "fail13.json",
        "fail14.json",

        "fail15.json",
        "fail16.json",

        "fail17.json",
        // "fail18.json", // TODO: verify if there is a limit on depth for json

        "fail19.json",
        "fail20.json",
        "fail21.json",
        "fail22.json",
        "fail23.json",
        "fail24.json",
        // "fail25.json", // TODO: validate if this is illegal
        // "fail26.json", // TODO: same as above
        // "fail27.json", // TODO: linebreak in key, fix this
        // "fail28.json", // TODO: linebreak in key, fix this

        "fail33.json"
    )
);

TEST_P(JsonCheckerPassesTests, passes) {
   std::ifstream file(path);
   ASSERT_TRUE(file.is_open());
   EXPECT_NO_THROW(libjson::parse(file));
}
INSTANTIATE_TEST_SUITE_P(
    JsonCheckerTestSuite,
    JsonCheckerPassesTests,
    ::testing::Values("pass1.json", "pass2.json", "pass3.json")
);
