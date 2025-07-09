#include "libjson/json-types.h"
#include "libjson/lexer.h"
#include "libjson/parse.h"
#include <gtest/gtest.h>

using namespace libjson;
TEST(JsonArray, const_ref_iterator) {
   std::istringstream stream(R"([1,2,3])");
   JsonValue          json = parse(stream);

   const JsonArray &array = json.get<JsonArray>();

   size_t i = 0;
   EXPECT_EQ(array.size(), 3);

   for (const auto &v : array) {
      EXPECT_EQ(array.get<int>(i), i + 1);
      i++;
   }
}

TEST(JsonArray, ref_iterator) {
   std::istringstream stream(R"([1,2,3])");
   JsonValue          json  = parse(stream);
   JsonArray         &array = json.get<JsonArray>();

   size_t i = 0;
   EXPECT_EQ(array.size(), 3);
   for (auto &v : array) {
      EXPECT_EQ(array.get<int>(i), i + 1);
      i++;
   }
}
