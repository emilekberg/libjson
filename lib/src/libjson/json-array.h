#pragma once
#include "json-value.h"
#include <vector>
namespace libjson {
class JSONArray {
public:
  void add(JSONValue &value) { _data.emplace_back(value); }
  std::vector<JSONValue>::iterator begin() { return _data.begin(); }
  std::vector<JSONValue>::iterator end() { return _data.end(); }

  JSONValue &operator[](size_t index) { return _data[index]; }

  JSONValue &at(size_t index) { return _data[index]; }
  template <typename T> T &at(size_t index) { return _data[index].get<T>(); }

  size_t size() { return _data.size(); }

private:
  std::vector<JSONValue> _data;
};

} // namespace libjson
