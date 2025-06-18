#include <gtest/gtest.h>
#include <libjson/parse.h>
#include <sstream>

TEST(parse, number_int) {
  std::istringstream ss(R"(420 )");
  libjson::LazyTokenizer tokens(ss);
  libjson::Token token = tokens.next();
  libjson::JsonValue value = libjson::parseNumber(token);

  EXPECT_EQ(value.getType(), libjson::ValueType::NUMBER);
  EXPECT_EQ(value.get<int>(), 420);
}

TEST(parse, number_uint16_t) {

  std::istringstream ss(R"(65535 )");
  libjson::LazyTokenizer tokens(ss);
  libjson::Token token = tokens.next();
  libjson::JsonValue value = libjson::parseNumber(token);

  EXPECT_EQ(value.getType(), libjson::ValueType::NUMBER);
  EXPECT_EQ(value.get<uint16_t>(), 0xFFFF);
}
