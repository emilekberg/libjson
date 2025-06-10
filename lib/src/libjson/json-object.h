#pragma once
#include "json-value.h"
#include <string>
#include <unordered_map>
#include <vector>

namespace libjson {
using JsonObjectDataType = std::unordered_map<std::string, JsonValue>;

class JsonObject {
public:
  JsonObject() = default;
  JsonObject(const JsonObjectDataType &data) : _data(data) {};

  std::unordered_map<std::string, JsonValue>::iterator begin() {
    return _data.begin();
  }

  std::unordered_map<std::string, JsonValue>::iterator end() {
    return _data.end();
  }
  std::vector<std::string> &keys();

  bool has(const std::string &key) const;

  const JsonValue &getValue(const std::string &key) const;
  JsonValue &getValue(const std::string &key);

  template <typename T> const T get(const std::string &key) const {
    return getValue(key).get<T>();
  }

  template <typename T> T get(const std::string &key) {
    return getValue(key).get<T>();
  }

private:
  JsonObjectDataType _data;
  std::vector<std::string> _keys;
};
} // namespace libjson
