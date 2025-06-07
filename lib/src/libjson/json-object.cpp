#include "json-object.h"
#include <sstream>
#include <stdexcept>

namespace libjson {
void JSONObject::add(const std::string &key, JSONValue value) {
  data[key] = value;
}

bool JSONObject::has(const std::string &key) {
  const auto &value = data.find(key);
  return value != data.end();
}
JSONValue &JSONObject::getValue(const std::string &key2) {
  if (!has(key2)) {
    std::ostringstream oss;
    oss << "invalid argument 'key': value = " << key2;
    throw std::invalid_argument(oss.str());
  }
  const auto &value = data.find(key2);
  return value->second;
}

} // namespace libjson
