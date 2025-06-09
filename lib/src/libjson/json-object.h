#pragma once
#include "json-value.h"
#include "libjson/json-null.h"
#include "libjson/json-number.h"
#include <string>
#include <type_traits>
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
  template <typename T>
    requires std::is_arithmetic_v<T> && (!std::is_same_v<T, bool>)
  void add(const std::string &key, T value) {
    add(key, {JSONValueType::NUMBER, JSONNumber(std::to_string(value))});
  }

  template <typename T>
    requires(!std::is_arithmetic_v<T>) || (std::is_same_v<T, bool>)
  void add(const std::string &key, const T &value) {
    if constexpr (std::is_same<T, std::string>()) {
      add(key, {JSONValueType::STRING, value});
    } else if constexpr (std::is_same<T, bool>()) {
      add(key, {JSONValueType::BOOL, value});
    } else if constexpr (std::is_same_v<T, JSONNumber>()) {
      add(key, {JSONValueType::NUMBER, value});
    } else if constexpr (std::is_same_v<T, JSONObject>()) {
      add(key, {JSONValueType::OBJECT, value});
    } else if constexpr (std::is_same_v<T, JSONNull>()) {
      add(key, {JSONValueType::_NULL, value});
    }
  }
  const bool has(const std::string &key) const;

  const JSONValue &getValue(const std::string &key) const;
  JSONValue &getValue(const std::string &key);

  template <typename T> const T get(const std::string &key) const {
    return getValue(key).get<const T>();
  }

  template <typename T> T get(const std::string &key) {
    return getValue(key).get<T>();
  }

private:
  std::unordered_map<std::string, JSONValue> _data;
  std::vector<std::string> _keys;
};
} // namespace libjson
