#pragma once
#include "libjson/json-number.h"
#include <any>
#include <concepts>
#include <string>
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
  bool is(JSONValueType lhs) { return type == lhs; }

  JSONValueType type;
  std::any value;
};

template <> inline bool JSONValue::get<bool>() {
  return std::any_cast<bool>(value);
}
} // namespace libjson
