#include <gtest/gtest.h>
#include <libjson/parser.h>

TEST(LIBJSON, someTestCase) {
  bool b = libjson::parser::parse();
  ASSERT_EQ(b, true);
}
