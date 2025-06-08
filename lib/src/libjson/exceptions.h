#include <format>
#include <string>
namespace libjson {
class UnexpectedTokenException : public std::exception {
private:
  std::string message;

public:
  // Constructor accepting const char*
  UnexpectedTokenException(std::string got, std::string expected)
      : message(std::format("Expected {}, but got {}", expected, got)) {}

  // Override what() method, marked
  // noexcept for modern C++
  const char *what() const noexcept { return message.c_str(); }
};
} // namespace libjson
