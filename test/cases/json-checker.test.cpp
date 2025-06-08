#include "gtest/gtest.h"
#include <format>
#include <fstream>
#include <gtest/gtest.h>
#include <iterator>
#include <libjson/parse.h>
#include <stdexcept>

class JsonCheckerPassesTests : public ::testing::TestWithParam<std::string> {
protected:
  std::string filecontent;

  void SetUp() override {
    std::string path = std::format("data/json-checker/{}", GetParam());
    std::ifstream file(path);
    ASSERT_TRUE(file.is_open());

    filecontent.assign((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
  }
};

class JsonCheckerFailsTests : public JsonCheckerPassesTests {};

TEST_P(JsonCheckerFailsTests, fails) {
  EXPECT_FALSE(filecontent.empty());
  EXPECT_THROW(libjson::parse(filecontent), std::invalid_argument);
}
INSTANTIATE_TEST_SUITE_P(
    JsonCheckerTestSuite, JsonCheckerFailsTests,
    ::testing::Values(
        "fail1.json", "fail2.json", "fail3.json",
        // "fail4.json", // NOTE: trailing comma
        // "fail5.json", // NOTE: trailing comma. reconsider as this is two
        "fail6.json", "fail7.json", "fail8.json",

        // "fail9.json", // trailing comma
        "fail10.json", "fail11.json", "fail12.json",
        // "fail13.json", // TODO: invalidate number with leading zero.
        "fail14.json",

        // "fail15.json", // TODO: implement this, validates specific escapes

        "fail16.json",

        // "fail17.json", // TODO: implement this, validate specific escape

        // "fail18.json", // TODO: verify if there is a limit on depth for json

        "fail19.json", "fail20.json", "fail21.json", "fail22.json",
        "fail23.json",
        // "fail24.json", // TODO: strings can only have single quotes?
        // "fail25.json", // TODO: validate if this is illegal
        // "fail26.json", // TODO: same as above
        // "fail27.json", // TODO: linebreak in key, fix this
        // "fail28.json", // TODO: linebreak in key, fix this
        // "fail29.json", // TODO: exponent, should be fine?
        "fail30.json", "fail31.json", "fail32.json", "fail33.json"));

TEST_P(JsonCheckerPassesTests, fails) {
  EXPECT_FALSE(filecontent.empty());
  EXPECT_THROW(libjson::parse(filecontent), std::invalid_argument);
}
INSTANTIATE_TEST_SUITE_P(JsonCheckerTestSuite, JsonCheckerPassesTests,
                         ::testing::Values("pass1.json", "pass2.json",
                                           "pass3.json"));
