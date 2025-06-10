#pragma once
#include "concepts.h"
#include "json-number.h"
#include <any>
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
    // return _value;
    return std::any_cast<const T>(value);
  }

  template <concepts::Numeric T> T get() {
    if (type == JSONValueType::NUMBER) {
      // return static_cast<JSONNumber>(_value).get<T>();
      return std::any_cast<JSONNumber>(value).get<T>();
    }
    // return static_cast<T>(_value);
    return std::any_cast<T>(value);
  }

  template <concepts::Numeric T> T get() const {
    if (type == JSONValueType::NUMBER) {
      // return static_cast<JSONNumber>(_value).get<T>();
      return std::any_cast<JSONNumber>(value).get<T>();
    }
    // return static_cast<T>(_value);
    return std::any_cast<T>(value);
  }
  const bool is(JSONValueType lhs) const { return type == lhs; }
  const JSONValueType getType() const { return type; }

  JSONValueType type;
  std::any value;

  // std::variant<JSONObject, JSONArray, JSONNumber, JSONNull, std::string>
  // _value;
};

template <> inline bool JSONValue::get<bool>() {
  return std::any_cast<bool>(value);
}

} // namespace libjson
