#include "object.h"
#include <any>
#include <cassert>

namespace libjson {
void Object::add(const std::string &key, Value value) { data[key] = value; }

bool Object::has(const std::string &key) {
  const auto &value = data.find(key);
  return value != data.end();
}
Value &Object::get(const std::string &key) {
  assert(has(key));
  const auto &value = data.find(key);
  return value->second;
}

template <typename T> T Object::get(const std::string &key) {
  Value &value = get(key);
  return value.get<T>();
}
} // namespace libjson
