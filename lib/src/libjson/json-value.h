#pragma once
#include "libjson/json-number.h"
#include <any>
#include <concepts>
namespace libjson {
enum class JSONValueType {
  __ERROR__,
  STRING,
  NUMBER,
  OBJECT,
  ARRAY,
  BOOL,
  _NULL,
};
struct JSONValue {
  template <typename T> T get() {
    T result = std::any_cast<T>(value);
    return result;
  }

  template <typename T>
    requires std::integral<T> || std::floating_point<T>
  T get() {
    if (type == JSONValueType::NUMBER) {
      return std::any_cast<JSONNumber>(value).get<T>();
    }
    return std::any_cast<T>(value);
  }

  template <typename T>
    requires std::integral<T> || std::floating_point<T>
  const T &get() const {
    if (type == JSONValueType::NUMBER) {
      return std::any_cast<JSONNumber>(value).get<T>();
    }
    return std::any_cast<T>(value);
  }
  const bool is(JSONValueType lhs) const { return type == lhs; }
  const JSONValueType getType() const { return type; }

  JSONValueType type;
  std::any value;
};

template <> inline bool JSONValue::get<bool>() {
  return std::any_cast<bool>(value);
}
} // namespace libjson
