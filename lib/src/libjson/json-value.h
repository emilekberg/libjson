#pragma once
#include "libjson/json-number.h"
#include <any>
#include <concepts>
#include <type_traits>
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
  template <typename T> T get() { return std::any_cast<T>(value); }

  template <typename T> const T get() const {
    return std::any_cast<const T>(value);
  }
  // TODO: Format nicer
  template <typename T>
    requires std::is_arithmetic_v<T>
  T get() {
    if (type == JSONValueType::NUMBER) {
      return std::any_cast<JSONNumber>(value).get<T>();
    }
    return std::any_cast<T>(value);
  }

  // TODO: Format nicer
  template <typename T>
    requires std::is_arithmetic_v<T>
  const T get() const {
    if (type == JSONValueType::NUMBER) {
      return std::any_cast<JSONNumber>(value).get<const T>();
    }
    return std::any_cast<const T>(value);
  }
  const bool is(JSONValueType lhs) const { return type == lhs; }
  const JSONValueType getType() const { return type; }

  JSONValueType type;
  std::any value;
};

template <> inline bool JSONValue::get<bool>() {
  return std::any_cast<bool>(value);
}

template <> inline bool const JSONValue::get<bool>() const {
  return std::any_cast<const bool>(value);
}
} // namespace libjson
