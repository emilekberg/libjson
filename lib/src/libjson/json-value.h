#pragma once
#include <any>
#include <string>
namespace libjson {
enum class JSONValueType {
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
  bool is(JSONValueType lhs) { return type == lhs; }

  JSONValueType type;
  std::any value;
};
} // namespace libjson
