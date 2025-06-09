#include "libjson/json-number.h"
#include <gtest/gtest.h>
#include <libjson/json-null.h>

using namespace libjson;

TEST(JSONNumber, get_int) {
  JSONNumber number("4");
  EXPECT_EQ(number.get<int>(), 4);
}

TEST(JSONNumber, get_int_negative) {
  JSONNumber number("-52");
  EXPECT_EQ(number.get<int>(), -52);
}

TEST(JSONNumber, get_float) {
  JSONNumber number("3.14");
  EXPECT_EQ(number.get<float>(), 3.14f);
}

TEST(JSONNumber, get_uint16_t) {
  JSONNumber number("123");
  EXPECT_EQ(number.get<uint16_t>(), 123);
}

TEST(JSONNumber, const_int) {
  const JSONNumber number("42069");
  const int expected = 42069;
  EXPECT_EQ(number.get<int>(), expected);
}

TEST(JSONNumber, get_int_exponent) {
  // TODO: investigate this failing test
  GTEST_SKIP_("Failing, investigate");
  JSONNumber number("12e+4");
  EXPECT_EQ(number.get<int>(), 12e+4);
}
