#pragma once
#include "json-value.h"
#include <vector>
namespace libjson {
class JsonArray {
public:
  void add(JsonValue &value) { _data.emplace_back(value); }
  std::vector<JsonValue>::iterator begin() { return _data.begin(); }
  std::vector<JsonValue>::iterator end() { return _data.end(); }

  JsonValue &operator[](size_t index) { return _data[index]; }

  JsonValue &at(size_t index) { return _data[index]; }
  template <typename T> T &at(size_t index) { return _data[index].get<T>(); }
  template <typename T> T &at(size_t index) const {
    return _data[index].get<T>();
  }

  size_t size() { return _data.size(); }

private:
  std::vector<JsonValue> _data;
};

} // namespace libjson
