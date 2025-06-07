#include "parse.h"
#include "libjson/lexer.h"
#include "libjson/object.h"
#include "libjson/token_types.h"
#include "token.h"
#include <stdexcept>
#include <vector>
namespace libjson {
Object parse(const std::string &input) {
  std::vector<Token> tokens;
  Lexer lexer(input);
  Token token;
  while (true) {
    token = lexer.next();
    if (token.type == TokenTypes::ILLEGAL) {
      throw std::runtime_error("ILLEGAL TOKEN FOUND");
    }
    tokens.push_back(token);
    if (token.type == libjson::TokenTypes::END_OF_FILE) {
      break;
    }
  }
  Object result;

  return result;
}
} // namespace libjson
