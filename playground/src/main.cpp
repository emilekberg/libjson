#include "libjson/token.h"
#include "libjson/token_types.h"
#include <iostream>
#include <libjson/lexer.h>
int main() {
  auto lexer = libjson::Lexer("{}");
  libjson::Token t;
  while ((t = lexer.next()).type != libjson::TokenTypes::ILLEGAL) {
    std::cout << t.literal;
  }
  std::cout << "done!";
  return 0;
}
