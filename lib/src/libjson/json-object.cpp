#include "json-object.h"
#include <cassert>

namespace libjson {
void JSONObject::add(const std::string &key, JSONValue value) {
  data[key] = value;
}

bool JSONObject::has(const std::string &key) {
  const auto &value = data.find(key);
  return value != data.end();
}
JSONValue &JSONObject::get(const std::string &key) {
  assert(has(key));
  const auto &value = data.find(key);
  return value->second;
}

template <typename T> T JSONObject::get(const std::string &key) {
  JSONValue &value = get(key);
  return value.get<T>();
}
} // namespace libjson
