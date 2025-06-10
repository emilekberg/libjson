#include "libjson/json-number.h"
#include <gtest/gtest.h>
#include <libjson/json-value.h>
#include <libjson/parse.h>

TEST(parse, number_int) {
  libjson::LazyTokenizer tokens(R"(420)");
  libjson::Token token = tokens.next();
  libjson::JsonValue value = libjson::parseNumber(token);

  EXPECT_EQ(value.type, libjson::JsonValueType::NUMBER);
  EXPECT_EQ(value.get<int>(), 420);
}

TEST(parse, number_uint16_t) {
  libjson::LazyTokenizer tokens(R"(65535)");
  // we know first token is an array, so discard it.
  libjson::Token token = tokens.next();
  libjson::JsonValue value = libjson::parseNumber(token);

  EXPECT_EQ(value.type, libjson::JsonValueType::NUMBER);
  EXPECT_EQ(value.get<uint16_t>(), 0xFFFF);
}
