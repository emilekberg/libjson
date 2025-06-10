#pragma once
#include "concepts.h"
#include "json-number.h"
#include <any>
namespace libjson {
enum class JsonValueType {
  __ERROR__,
  STRING,
  NUMBER,
  OBJECT,
  ARRAY,
  BOOL,
  _NULL,
};
struct JsonValue {

  template <typename T> T get() { return std::any_cast<T>(value); }

  template <typename T> const T get() const {
    // return _value;
    return std::any_cast<const T>(value);
  }

  template <concepts::Numeric T> T get() {
    if (type == JsonValueType::NUMBER) {
      // return static_cast<JsonNumber>(_value).get<T>();
      return std::any_cast<JsonNumber>(value).get<T>();
    }
    // return static_cast<T>(_value);
    return std::any_cast<T>(value);
  }

  template <concepts::Numeric T> T get() const {
    if (type == JsonValueType::NUMBER) {
      // return static_cast<JsonNumber>(_value).get<T>();
      return std::any_cast<JsonNumber>(value).get<T>();
    }
    // return static_cast<T>(_value);
    return std::any_cast<T>(value);
  }
  const bool is(JsonValueType lhs) const { return type == lhs; }
  const JsonValueType getType() const { return type; }

  JsonValueType type;
  std::any value;

  // std::variant<JsonObject, JsonArray, JsonNumber, JsonNull, std::string>
  // _value;
};

template <> inline bool JsonValue::get<bool>() {
  return std::any_cast<bool>(value);
}

} // namespace libjson
