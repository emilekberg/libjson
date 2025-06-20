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
} // namespace libjson
