#include <fstream>
#include <iostream>
#include <libjson/parse.h>
#include <sstream>

static std::string loadFile(const std::string &path) {
  std::ifstream fstream(path);
  std::ostringstream ss;
  ss << fstream.rdbuf();
  return ss.str();
}

int main() {
  std::string json = loadFile("data/large-file.json");
  auto result = libjson::parse(json);

  return 0;
}
