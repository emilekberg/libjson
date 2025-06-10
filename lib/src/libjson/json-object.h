#pragma once
#include "concepts.h"
#include "json-null.h"
#include "json-number.h"
#include "json-value-types.h"
#include "json-value.h"
#include <string>
#include <unordered_map>
#include <vector>

namespace libjson {
class JsonObject {
public:
  std::unordered_map<std::string, JsonValue>::iterator begin() {
    return _data.begin();
  }

  std::unordered_map<std::string, JsonValue>::iterator end() {
    return _data.end();
  }
  std::vector<std::string> &keys();

  template <concepts::Numeric T> void add(const std::string &key, T value) {
    add(key, {JsonValueType::NUMBER, JsonNumber(std::to_string(value))});
  }

  template <concepts::NonNumeric T>
  void add(const std::string &key, const T &value) {
    if constexpr (std::is_same_v<T, std::string>) {
      add(key, {JsonValueType::STRING, value});
    } else if constexpr (std::is_same_v<T, bool>) {
      add(key, {JsonValueType::BOOL, value});
    } else if constexpr (std::is_same_v<T, JsonNumber>) {
      add(key, {JsonValueType::NUMBER, value});
    } else if constexpr (std::is_same_v<T, JsonObject>) {
      add(key, {JsonValueType::OBJECT, value});
    } else if constexpr (std::is_same_v<T, JsonNull>) {
      add(key, {JsonValueType::_NULL_, value});
    }
  }

  void add(const std::string &key, JsonValue value);
  bool has(const std::string &key) const;

  const JsonValue &getValue(const std::string &key) const;
  JsonValue &getValue(const std::string &key);

  template <typename T> T get(const std::string &key) const {
    return getValue(key).get<T>();
  }

  template <typename T> T get(const std::string &key) {
    return getValue(key).get<T>();
  }

private:
  std::unordered_map<std::string, JsonValue> _data;
  std::vector<std::string> _keys;
};
} // namespace libjson
