#include "libjson/json-array.h"
#include <gtest/gtest.h>
#include <libjson/parse.h>
#include <vector>

TEST(parse, array_of_numbers) {
  std::string input = R"([1,2,4,8,16,32,64])";

  std::vector<double> expected_arr = {1, 2, 4, 8, 16, 32, 64};
  libjson::Tokenizer tokens = libjson::tokenize(input);
  // we know first token is an array, so discard it.
  tokens.next();
  libjson::JSONArray array = libjson::parseArray(tokens);

  ASSERT_EQ(array.size(), 7);

  for (size_t i = 0; i < array.size(); i++) {
    double expected = expected_arr[i];
    double actual = array[i].get<double>();

    EXPECT_EQ(actual, expected);
  }
}
