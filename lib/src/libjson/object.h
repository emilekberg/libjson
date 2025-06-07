#pragma once
#include <any>
#include <string>
namespace libjson {
enum class ValueType {
  STRING,
  NUMBER,
  OBJECT,
  ARRAY,
  BOOL,
  _NULL,
};
class Value {
public:
  ValueType type;
  std::any value;
};
class Object {
public:
  bool has(const std::string &key);
  Value get(const std::string &key);
};
} // namespace libjson
