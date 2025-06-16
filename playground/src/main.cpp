#include "libjson/json-types.h"
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
  std::string json = loadFile("data/00.json");
  auto result = libjson::parse(json);
  auto val = result.get<libjson::JsonArray>().size();
  return val;
}
