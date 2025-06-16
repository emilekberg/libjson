#include "libjson/json-types.h"
#include "libjson/lexer.h"
#include "libjson/token.h"
#include "libjson/token_types.h"
#include "libjson/tokenizer.h"
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
  const std::string json = loadFile("data/00.json");
  for (size_t i = 0; i < 10; i++) {
    libjson::LazyTokenizer tokens(json);
    while (tokens.next() != libjson::Token_EndOfFile) {
    }
  }
  return 0;
}
