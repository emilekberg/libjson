#include "json-object.h"
#include <format>
#include <stdexcept>

namespace libjson {

std::vector<std::string> &JsonObject::keys() { return _keys; }

bool JsonObject::has(const std::string &key) const {
  const auto &value = _data.find(key);
  return value != _data.end();
}
JsonValue &JsonObject::getValue(const std::string &key) {
  auto value = _data.find(key);
  if (value == _data.end()) {
    throw std::invalid_argument(
        std::format("JsonObject: does not contain object with key: {}", key));
  }
  return value->second;
}

const JsonValue &JsonObject::getValue(const std::string &key) const {
  const auto &value = _data.find(key);
  if (value == _data.end()) {
    throw std::invalid_argument(
        std::format("JsonObject: does not contain object with key: {}", key));
  }
  return value->second;
}

} // namespace libjson
