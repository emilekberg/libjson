#include "json-object.h"
#include <format>
#include <stdexcept>

namespace libjson {

std::vector<std::string> &JSONObject::keys() { return _keys; }

void JSONObject::add(const std::string &key, JSONValue value) {
  _data.emplace(key, value);
  _keys.emplace_back(key);
}

const bool JSONObject::has(const std::string &key) const {
  const auto &value = _data.find(key);
  return value != _data.end();
}
JSONValue &JSONObject::getValue(const std::string &key) {
  auto value = _data.find(key);
  if (value == _data.end()) {
    throw std::invalid_argument(
        std::format("JSONObject: does not contain object with key: {}", key));
  }
  return value->second;
}

const JSONValue &JSONObject::getValue(const std::string &key) const {
  const auto &value = _data.find(key);
  if (value == _data.end()) {
    throw std::invalid_argument(
        std::format("JSONObject: does not contain object with key: {}", key));
  }
  return value->second;
}

} // namespace libjson
