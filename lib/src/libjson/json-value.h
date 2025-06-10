#pragma once
#include "concepts.h"
#include "json-number.h"
#include "json-value-types.h"
#include <any>
namespace libjson {
struct JsonValue {
  template <typename T> T get() { return std::any_cast<T>(value); }

  template <typename T> const T get() const {
    return std::any_cast<const T>(value);
  }

  template <concepts::Numeric T> T get() {
    return std::any_cast<JsonNumber>(value).get<T>();
  }

  template <concepts::Numeric T> T get() const {
    return std::any_cast<T>(value);
  }
  const bool is(JsonValueType lhs) const { return type == lhs; }
  const JsonValueType getType() const { return type; }

  JsonValueType type;
  std::any value;
};

template <> inline bool JsonValue::get<bool>() {
  return std::any_cast<bool>(value);
}

} // namespace libjson
