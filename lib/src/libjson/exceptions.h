#include "token.h"
#include <exception>
#include <format>
#include <string>
#include <string_view>
#include <vector>
namespace libjson {

class unexpected_token final : public std::exception {
private:
  std::string message;

public:
  unexpected_token(const std::string &msg) : message(msg) {}

  unexpected_token(const std::string &got, const std::string &expected)
      : message(std::format("Expected {}, but got {}", expected, got)) {}

  template <typename... Expected>
  static unexpected_token from_variadic(const std::string &got,
                                        Expected &&...expected) {
    std::vector<std::string_view> expectedVector;
    (expectedVector.push_back(expected.literal), ...);

    std::string expectedMessage = "";
    for (size_t i = 0; i < expectedVector.size(); i++) {
      if (i > 0) {
        expectedMessage += " or ";
      }
      expectedMessage += expectedVector[i];
    }
    return unexpected_token(got, std::move(expectedMessage));
  }

  const char *what() const noexcept override { return message.c_str(); }
};

static inline void expect_token(const Token &actual, const Token &expected) {
  if (actual != expected) [[unlikely]]
    throw unexpected_token(actual.literal, expected.literal);
}

template <typename... Tokens>
static inline void expect_tokens(const Token &actual,
                                 const Tokens &...expected) {
  if (((actual != expected) && ...)) [[unlikely]] {
    throw unexpected_token::from_variadic(actual.literal, expected...);
  }
}
} // namespace libjson
