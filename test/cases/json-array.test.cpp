#include "libjson/json-types.h"
#include <gtest/gtest.h>

using namespace libjson;
TEST(JsonArray, const_ref_iterator) {
   JsonArray array({JsonNumber("1"), JsonNumber("2"), JsonNumber("3")});

   size_t i = 0;
   EXPECT_EQ(array.size(), 3);
   for (const auto &v : array) {
      EXPECT_EQ(array.get<int>(i), i + 1);
      i++;
   }
}

TEST(JsonArray, ref_iterator) {
   JsonArray array({JsonNumber("1"), JsonNumber("2"), JsonNumber("3")});

   size_t i = 0;
   EXPECT_EQ(array.size(), 3);
   for (auto &v : array) {
      EXPECT_EQ(array.get<int>(i), i + 1);
      i++;
   }
}

TEST(JsonArray, iterator) {
   JsonArray array({JsonNumber("1"), JsonNumber("2"), JsonNumber("3")});

   size_t i = 0;
   EXPECT_EQ(array.size(), 3);
   for (auto v : array) {
      EXPECT_EQ(v.get<int>(), i + 1);
      i++;
   }
}
