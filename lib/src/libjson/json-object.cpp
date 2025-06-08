#include "json-object.h"
#include <sstream>
#include <stdexcept>

namespace libjson {

std::vector<std::string> &JSONObject::getKeys() { return _keys; }

void JSONObject::add(const std::string &key, JSONValue value) {
  _data[key] = value;
  _keys.emplace_back(key);
}

bool JSONObject::has(const std::string &key) {
  const auto &value = _data.find(key);
  return value != _data.end();
}
JSONValue &JSONObject::getValue(const std::string &key) {
  if (!has(key)) {
    std::ostringstream oss;
    oss << "invalid argument 'key': value = " << key;
    throw std::invalid_argument(oss.str());
  }
  const auto &value = _data.find(key);
  return value->second;
}

} // namespace libjson
