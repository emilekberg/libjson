#include "token.h"
#include <format>
#include <string>
namespace libjson {

class unexpected_token : public std::exception {
private:
  std::string message;

public:
  unexpected_token(const std::string &got, const std::string &expected)
      : message(std::format("Expected {}, but got {}", expected, got)) {}

  unexpected_token(const std::string &got, const std::string &expected1,
                   const std::string &expected2)
      : message(std::format("Expected {} or {}, but got {}", expected1,
                            expected2, got)) {}

  const char *what() const noexcept { return message.c_str(); }
};

static inline void expect_token(const Token &actual, const Token &expected) {
  if (actual != expected) [[unlikely]]
    throw unexpected_token(actual.literal, expected.literal);
}

static inline void expect_token(const Token &actual,
                                const Token &expected_first,
                                const Token &expected_second) {
  if (actual != expected_first && actual != expected_second) [[unlikely]]
    throw unexpected_token(actual.literal, expected_first.literal,
                           expected_second.literal);
}
} // namespace libjson
