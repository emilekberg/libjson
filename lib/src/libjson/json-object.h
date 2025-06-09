#pragma once
#include "json-value.h"
#include <string>
#include <unordered_map>
#include <vector>
namespace libjson {
class JSONObject {
public:
  std::unordered_map<std::string, JSONValue>::iterator begin() {
    return _data.begin();
  }

  std::unordered_map<std::string, JSONValue>::iterator end() {
    return _data.end();
  }

  std::vector<std::string> &keys();
  void add(const std::string &key, JSONValue value);
  const bool has(const std::string &key) const;

  const JSONValue &getValue(const std::string &key) const;
  JSONValue &getValue(const std::string &key);

  template <typename T> const T get(const std::string &key) const {
    return getValue(key).get<T>();
  }

  template <typename T> T get(const std::string &key) {
    return getValue(key).get<T>();
  }

private:
  std::unordered_map<std::string, JSONValue> _data;
  std::vector<std::string> _keys;
};
} // namespace libjson
