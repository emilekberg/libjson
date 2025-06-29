#include <gtest/gtest.h>
#include <libjson/parse.h>
#include <vector>

TEST(parse, array_of_numbers) {
   std::istringstream input(R"([1,2,4,8,16,32,64])");

   std::vector<double> expected_arr = {1, 2, 4, 8, 16, 32, 64};
   libjson::Lexer      lexer(input);
   lexer.next();
   libjson::JsonArray array = libjson::parseArray(lexer);

   ASSERT_EQ(array.size(), expected_arr.size());

   for (size_t i = 0; i < array.size(); i++) {
      double expected = expected_arr[i];
      double actual   = array[i].get<double>();

      EXPECT_EQ(actual, expected);
   }
}
