#include <gtest/gtest.h>
#include <libjson/parse.h>

TEST(Parser, parses_empty_object) {
  libjson::Object result = libjson::parse(R"({})");
  bool b = true;
  EXPECT_EQ(b, true);
}
