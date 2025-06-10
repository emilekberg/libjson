#pragma once
#include "concepts.h"
#include "json-null.h"
#include "json-number.h"
#include "json-value.h"
#include <string>
#include <unordered_map>
#include <vector>

namespace libjson {
class JSONValue;
class JSONObject {
public:
  std::unordered_map<std::string, JSONValue>::iterator begin() {
    return _data.begin();
  }

  std::unordered_map<std::string, JSONValue>::iterator end() {
    return _data.end();
  }
  std::vector<std::string> &keys();

  template <concepts::Numeric T> void add(const std::string &key, T value) {
    add(key, {JSONValueType::NUMBER, JSONNumber(std::to_string(value))});
  }

  template <concepts::NonNumeric T>
  void add(const std::string &key, const T &value) {
    if constexpr (std::is_same_v<T, std::string>) {
      add(key, {JSONValueType::STRING, value});
    } else if constexpr (std::is_same_v<T, bool>) {
      add(key, {JSONValueType::BOOL, value});
    } else if constexpr (std::is_same_v<T, JSONNumber>) {
      add(key, {JSONValueType::NUMBER, value});
    } else if constexpr (std::is_same_v<T, JSONObject>) {
      add(key, {JSONValueType::OBJECT, value});
    } else if constexpr (std::is_same_v<T, JSONNull>) {
      add(key, {JSONValueType::_NULL, value});
    }
  }

  void add(const std::string &key, JSONValue value);
  bool has(const std::string &key) const;

  const JSONValue &getValue(const std::string &key) const;
  JSONValue &getValue(const std::string &key);

  template <typename T> T get(const std::string &key) const {
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
