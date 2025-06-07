#pragma once
#include <any>
#include <string>
#include <unordered_map>
namespace libjson {
enum class ValueType {
  STRING,
  NUMBER,
  OBJECT,
  ARRAY,
  BOOL,
  _NULL,
};
struct Value {
  template <typename T> T get() {
    T result = std::any_cast<T>(value);
    return result;
  }

  ValueType type;
  std::any value;
};
class Object {
public:
  void add(const std::string &key, Value value);
  bool has(const std::string &key);
  Value &get(const std::string &key);
  template <typename T> T get(const std::string &key);

  std::unordered_map<std::string, Value> data;
};
} // namespace libjson
