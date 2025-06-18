#include "libjson/json-types.h"
#include "libjson/lexer.h"
#include "libjson/token.h"
#include "libjson/token_types.h"
#include "libjson/tokenizer.h"
#include <cassert>
#include <fstream>
#include <iostream>
// #include <libjson/json-object.h>
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
  // const std::string json = R"(1234.56)";
  // const std::string json = loadFile("data/json-checker/pass01.json");
  std::ifstream stream("data/json-checker/pass1.json");

  assert(stream.good());
  libjson::JsonValue json = libjson::parse(stream);
  // std::istringstream stream(R"({"numbers":[1, 2, 3,]})");
  // libjson::LazyTokenizer tokens(stream);
  // for (const auto token : Lexer(stream)) {
  //   if (token.type == TokenTypes::ILLEGAL) {
  //     return -1;
  //   }
  //   std::cout << token.literal << std::endl;
  // }
  return 0;
}
