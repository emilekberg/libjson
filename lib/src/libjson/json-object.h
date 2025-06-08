#pragma once
#include "json-value.h"
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>
namespace libjson {
class JSONObject {
public:
  // struct Iterator {
  //   using iterator_category = std::forward_iterator_tag;
  //   using difference_type = std::ptrdiff_t;
  //   using value_type = JSONValue;
  //   using pointer = value_type *;
  //   using reference = value_type &;
  //
  //   Iterator(pointer pointer) : _pointer(pointer) {};
  //
  //   reference operator*() const { return *_pointer; }
  //   pointer operator->() { return _pointer; }
  //
  //   Iterator &operator++() {
  //     _pointer++;
  //     return *this;
  //   }
  //   Iterator operator++(int) {
  //     Iterator temp = *this;
  //     ++(*this);
  //     return temp;
  //   }
  //
  //   friend bool operator==(const Iterator &a, const Iterator &b) {
  //     return a._pointer == b._pointer;
  //   };
  //   friend bool operator!=(const Iterator &a, const Iterator &b) {
  //     return a._pointer != b._pointer;
  //   };
  //
  // private:
  //   pointer _pointer;
  // };

  std::unordered_map<std::string, JSONValue>::iterator begin() {
    return _data.begin();
  }

  std::unordered_map<std::string, JSONValue>::iterator end() {
    return _data.end();
  }
  // Iterator begin() { return Iterator(&_data.begin()); }
  // Iterator end() { return Iterator(&_data.end()); }

  std::vector<std::string> &getKeys();
  void add(const std::string &key, JSONValue value);
  bool has(const std::string &key);
  JSONValue &getValue(const std::string &key);

  template <typename T> T get(const std::string &key) {
    JSONValue &value = getValue(key);
    return value.get<T>();
  }

private:
  std::unordered_map<std::string, JSONValue> _data;
  std::vector<std::string> _keys;
};
} // namespace libjson
