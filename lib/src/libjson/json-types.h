#pragma once

#include "concepts.h"
#include "json-value-types.h"
#include <charconv>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <variant>
#include <vector>

namespace libjson {

class JsonNumber {
public:
  JsonNumber(const std::string &literal) : _literal(literal) {}

  template <libjson::concepts::Numeric T> T get() {
    T value;
    auto result = std::from_chars(_literal.data(),
                                  _literal.data() + _literal.size(), value);
    if (result.ec != std::errc{}) {
      throw std::invalid_argument("Invalid integer: " + _literal);
    }
    return value;
  }
  template <libjson::concepts::Numeric T> T get() const {
    T value;
    auto result = std::from_chars(_literal.data(),
                                  _literal.data() + _literal.size(), value);
    if (result.ec != std::errc{}) {
      throw std::invalid_argument("Invalid integer: " + _literal);
    }
    return value;
  }

private:
  std::string _literal;
};
using JsonBool = bool;
using JsonString = std::string;
using JsonNull = std::nullptr_t;

// forward declare
class JsonObject;
class JsonArray;

// json value needs to store JsonObject and JsonArray in shared_ptr to know the
// size of it at compile time.
//
struct JsonValue {
  using VariantType = std::variant<std::monostate, std::shared_ptr<JsonArray>,
                                   std::shared_ptr<JsonObject>, JsonNull,
                                   JsonString, JsonBool, JsonNumber>;
  VariantType value;
  JsonValueType type;

  JsonValue() = default;
  bool is(JsonValueType lhs) { return type == lhs; }
  JsonValueType getType() const { return type; }
  // construct simple types trivially
  template <typename T>
    requires(!std::same_as<std::decay_t<T>, JsonArray> &&
             !std::same_as<std::decay_t<T>, JsonObject>)

  JsonValue(T &&v) : value(std::forward<T>(v)) {
    if constexpr (std::is_same_v<T, JsonBool>) {
      type = JsonValueType::BOOL;
    } else if constexpr (std::is_same_v<T, JsonString>) {
      type = JsonValueType::STRING;
    } else if constexpr (std::is_same_v<T, JsonNull>) {
      type = JsonValueType::_NULL_;
    } else if constexpr (std::is_same_v<T, JsonNumber>) {
      type = JsonValueType::NUMBER;
    }
  }

  // since we have to store arrays and objects in shared pointers
  // we have special constructors for them so the user does not need to care
  // about that.
  JsonValue(const JsonArray &array)
      : value(std::make_shared<JsonArray>(array)), type(JsonValueType::ARRAY) {}
  JsonValue(JsonArray &array)
      : value(std::make_shared<JsonArray>(std::move(array))),
        type(JsonValueType::ARRAY) {}
  JsonValue(const JsonObject &object)
      : value(std::make_shared<JsonObject>(object)),
        type(JsonValueType::OBJECT) {}
  JsonValue(JsonObject &object)
      : value(std::make_shared<JsonObject>(std::move(object))),
        type(JsonValueType::OBJECT) {}

  template <libjson::concepts::Numeric T> T get() const {
    if (auto p = std::get_if<JsonNumber>(&value)) {
      return (*p).get<T>();
    }
    throw std::runtime_error("oh shit not good");
  }

  template <libjson::concepts::NonNumeric T> T get() const {
    if constexpr (std::is_same_v<T, JsonArray>) {
      if (auto p = std::get_if<std::shared_ptr<JsonArray>>(&value)) {
        return **p;
      }
    } else if constexpr (std::is_same_v<T, JsonObject>) {
      if (auto p = std::get_if<std::shared_ptr<JsonObject>>(&value)) {
        return **p;
      }
    } else {
      if (auto p = std::get_if<T>(&value)) {
        return *p;
      }
    }
    throw std::runtime_error("oh shit not good");
  }
};
using JsonArrayData = std::vector<JsonValue>;
class JsonArray {
public:
  JsonArray() = default;
  JsonArray(const JsonArrayData &data) : _data(data) {};

  template <libjson::concepts::Numeric T> T get(size_t index) {
    return _data.at(index).get<T>();
  }

  template <libjson::concepts::NonNumeric T> T get(size_t index) {
    return _data.at(index).get<T>();
  }

  JsonArrayData _data;
};

using JsonObjectData = std::unordered_map<std::string, JsonValue>;
class JsonObject {
public:
  JsonObject() = default;
  JsonObject(const JsonObjectData &data) : _data(data) {};

  template <libjson::concepts::Numeric T> T get(const std::string &key) {
    auto it = _data.find(key);
    if (it == _data.end()) {
      throw std::runtime_error("key not found: " + key);
    }
    return _data.at(key).get<T>();
  }

  template <libjson::concepts::Numeric T> T get(const std::string &key) const {
    auto it = _data.find(key);
    if (it == _data.end()) {
      throw std::runtime_error("key not found: " + key);
    }
    return _data.at(key).get<T>();
  }

  template <libjson::concepts::NonNumeric T>
  const T &get(const std::string &key) const {
    auto it = _data.find(key);
    if (it == _data.end()) {
      throw std::runtime_error("key not found: " + key);
    }

    if constexpr (std::is_same_v<T, JsonValue>) {
      return _data.at(key);
    } else if constexpr (std::is_same_v<T, JsonString>) {
      return _data.at(key).get<T>();
    } else {
      return it->second.get<T>();
    }
  }

  template <libjson::concepts::NonNumeric T>
    requires std::is_trivially_copyable_v<T>
  const T get(const std::string &key) const {
    auto it = _data.find(key);
    if (it == _data.end()) {
      throw std::runtime_error("key not found: " + key);
    }
    if constexpr (std::is_same_v<T, JsonValue>) {
      return _data.at(key);
    }
    return it->second.get<T>();
  }

  bool has(const std::string key) { return _data.find(key) != _data.end(); }
  bool has(const std::string key) const {
    return _data.find(key) != _data.end();
  }

private:
  JsonObjectData _data;
};

} // namespace libjson
