#include <format>
#include <string>
namespace libjson {
class unexpected_token : public std::exception {
private:
  std::string message;

public:
  unexpected_token(std::string got, std::string expected)
      : message(std::format("Expected {}, but got {}", expected, got)) {}

  const char *what() const noexcept { return message.c_str(); }
};
} // namespace libjson
