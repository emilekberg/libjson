#include <fstream>
#include <iostream>
#include <libjson/json-object.h>
#include <libjson/parse.h>
#include <sstream>

static std::string loadFile(const std::string &path) {
  std::ifstream fstream(path);
  std::ostringstream ss;
  ss << fstream.rdbuf();
  return ss.str();
}

int main() {
  // std::string json = loadFile("data/twitter.json");
  //    std::string json = R"({
  //      "public": true,
  //      "created_at": "2015-01-01T15:00:00Z",
  //  })";
  libjson::JSONValue val = libjson::parse(R"(["<-- missing value"],)");
  // auto val = result.get<libjson::JSONObject>();
  return 0;
}
