#pragma once
#include "json-value.h"
#include <string>
#include <unordered_map>
namespace libjson {
class JSONObject {
public:
  void add(const std::string &key, JSONValue value);
  bool has(const std::string &key);
  JSONValue &get(const std::string &key);
  template <typename T> T get(const std::string &key);

  std::unordered_map<std::string, JSONValue> data;
};
} // namespace libjson
