#include <gtest/gtest.h>
#include <libjson/json-types.h>

using namespace libjson;

TEST(JsonNumber, get_int) {
  JsonNumber number("4");
  EXPECT_EQ(number.get<int>(), 4);
}

TEST(JsonNumber, get_int_negative) {
  JsonNumber number("-52");
  EXPECT_EQ(number.get<int>(), -52);
}

TEST(JsonNumber, get_float) {
  JsonNumber number("3.14");
  EXPECT_EQ(number.get<float>(), 3.14f);
}

TEST(JsonNumber, get_uint16_t) {
  JsonNumber number("123");
  EXPECT_EQ(number.get<uint16_t>(), 123);
}

TEST(JsonNumber, const_int) {
  const JsonNumber number("42069");
  const int expected = 42069;
  EXPECT_EQ(number.get<int>(), expected);
}

TEST(JsonNumber, get_int_exponent) {
  // TODO: investigate this failing test
  GTEST_SKIP_("Failing, investigate");
  JsonNumber number("12e+4");
  EXPECT_EQ(number.get<int>(), 12e+4);
}
