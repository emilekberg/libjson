#include "libjson/json-types.h"
#include "libjson/lexer.h"
#include "libjson/token.h"
#include "libjson/token_types.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <libjson/concepts.h>
#include <libjson/parse.h>
#include <sstream>

static std::string loadFile(const std::string &path) {
  std::ifstream fstream(path);
  std::ostringstream ss;
  ss << fstream.rdbuf();
  return ss.str();
}

using namespace libjson;
int main() {
  std::istringstream stream(R"({"numbers":[1, 2, 3,]})");
  // const std::string json = loadFile("data/json-checker/pass01.json");
  // std::ifstream stream("data/json-checker/pass1.json");

  // assert(stream.good());
  // libjson::JsonValue json = libjson::parse(stream);
  // std::istringstream stream(R"({"numbers":[1, 2, 3,]})");
  for (const auto token : Lexer(stream)) {
    std::cout << token.literal << std::endl;
    if (token.type == TokenTypes::ILLEGAL) {
      return -1;
    }
  }
  return 0;
}
