#pragma once
#include "json-value.h"
#include <string>
#include <unordered_map>
namespace libjson {
class JSONObject {
public:
  void add(const std::string &key, JSONValue value);
  bool has(const std::string &key);
  JSONValue &getValue(const std::string &key);

  template <typename T> T get(const std::string &key) {
    JSONValue &value = getValue(key);
    return value.get<T>();
  }

  std::unordered_map<std::string, JSONValue> data;
};
} // namespace libjson
