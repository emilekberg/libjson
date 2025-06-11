#include <fstream>
#include <iostream>
// #include <libjson/json-object.h>
// #include <libjson/parse.h>
#include "types.h"
#include <libjson/concepts.h>
#include <sstream>

static std::string loadFile(const std::string &path) {
  std::ifstream fstream(path);
  std::ostringstream ss;
  ss << fstream.rdbuf();
  return ss.str();
}

int main() {
  JsonArray arr1({JsonNumber("1"), JsonNumber("3")});
  JsonArray arr({true, JsonNumber("13.37"), arr1});
  if (arr.get<bool>(0)) {
    float f = arr.get<float>(1);
    for (const auto &v : arr.get<JsonArray>(2)._data) {
      std::cout << v.get<int>() << std::endl;
    }
    return 0;
  }
  // std::string json = loadFile("data/large-file.json");
  //    std::string json = R"({
  //      "public": true,
  //      "created_at": "2015-01-01T15:00:00Z",
  //  })";
  // libjson::parse(json);
  // auto val = result.get<libjson::JsonObject>();
  return 0;
}
