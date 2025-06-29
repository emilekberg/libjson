#pragma once

#include "concepts.h"
#include <charconv>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <variant>
#include <vector>

namespace libjson {

enum class ValueType {
   _NULL_,
   STRING,
   NUMBER,
   OBJECT,
   ARRAY,
   BOOL,
};

//--------------------
// basic types
// - string
// - bool
// - null
//--------------------
using JsonString = std::string;
using JsonBool   = bool;
using JsonNull   = std::nullptr_t;
// forward declare types since we have a recursion going on
// Objects has values, which can be objects.
// Arrays has values, which can be arrays.
class JsonObject;
class JsonArray;

//--------------------
// advanced types
// - number
// - value (variant of all types)
//--------------------
class JsonNumber {
 public:
   JsonNumber(const std::string &literal) {

      auto result = std::from_chars(literal.data(), literal.data() + literal.size(), _int);

      if (result.ec != std::errc{}) {
         throw std::invalid_argument("Invalid integer: " + literal);
      }
      result = std::from_chars(literal.data(), literal.data() + literal.size(), _double);
      if (result.ec != std::errc{}) {
         throw std::invalid_argument("Invalid double: " + literal);
      }
   }

   template <concepts::Int T> T get() const {
      return static_cast<T>(_int);
   }
   template <concepts::Float T> T get() const {
      return static_cast<T>(_double);
   }

 private:
   int64_t _int;
   double  _double;
};

// json value needs to store JsonObject and JsonArray in shared_ptr to know the
// size of it at compile time.
class JsonValue {
 public:
   using JsonValueVariantType = std::variant<
       std::monostate,
       std::shared_ptr<JsonArray>,
       std::shared_ptr<JsonObject>,
       JsonNull,
       JsonString,
       JsonBool,
       JsonNumber>;

   JsonValue() noexcept = default;

   // copy constructors.
   JsonValue(const JsonValue &) noexcept       = default;
   JsonValue(JsonValue &&) noexcept            = default;
   JsonValue &operator=(const JsonValue &)     = default;
   JsonValue &operator=(JsonValue &&) noexcept = default;

   // For most objects, we can create value with a simple constructor.
   template <typename T>
      requires(
          !std::same_as<std::decay_t<T>, JsonArray> && !std::same_as<std::decay_t<T>, JsonObject> &&
          !std::same_as<std::decay_t<T>, JsonValue>
      )
   JsonValue(T &&v) : _value(std::forward<T>(v)) {
      // deduce the type from, for instance, const std::string& to just the type.
      using U = std::decay_t<T>;
      if constexpr (std::is_same_v<U, JsonBool>) {
         _type = ValueType::BOOL;
      } else if constexpr (std::is_same_v<U, JsonNull>) {
         _type = ValueType::_NULL_;
      } else if constexpr (std::is_same_v<U, JsonNumber>) {
         _type = ValueType::NUMBER;
      } else if constexpr (std::is_convertible_v<U, JsonString>) {
         _type = ValueType::STRING;
      } else {
         static_assert(false, "JsonValue: unsupported type");
      }
   }

   // since we have to store arrays and objects in shared pointers
   // we have special constructors for them so the user does not need to care
   // about that.
   JsonValue(const JsonArray &array)
       : _value(std::make_shared<JsonArray>(array)), _type(ValueType::ARRAY) {}
   JsonValue(JsonArray &array)
       : _value(std::make_shared<JsonArray>(std::move(array))), _type(ValueType::ARRAY) {}
   JsonValue(JsonArray &&array)
       : _value(std::make_shared<JsonArray>(std::move(array))), _type(ValueType::ARRAY) {}

   JsonValue(const JsonObject &object)
       : _value(std::make_shared<JsonObject>(object)), _type(ValueType::OBJECT) {}
   JsonValue(JsonObject &object)
       : _value(std::make_shared<JsonObject>(std::move(object))), _type(ValueType::OBJECT) {}
   JsonValue(JsonObject &&object)
       : _value(std::make_shared<JsonObject>(std::move(object))), _type(ValueType::OBJECT) {}

   ValueType getType() const {
      return _type;
   }
   bool is(ValueType lhs) const {
      return _type == lhs;
   }

   template <libjson::concepts::Numeric T> T get() const {
      if (auto p = std::get_if<JsonNumber>(&_value)) {
         return (*p).get<T>();
      }
      throw std::invalid_argument("JsonValue::get<Numeric> variant not satisfied.");
   }

   // extracts the pointers to ensure the user does not have to care about it.
   // for simple values we just return the contained type.
   template <libjson::concepts::NonNumeric T> T get() const {
      if constexpr (std::is_same_v<T, JsonArray>) {
         if (auto p = std::get_if<std::shared_ptr<JsonArray>>(&_value)) {
            return **p;
         }
      } else if constexpr (std::is_same_v<T, JsonObject>) {
         if (auto p = std::get_if<std::shared_ptr<JsonObject>>(&_value)) {
            return **p;
         }
      } else {
         if (auto p = std::get_if<T>(&_value)) {
            return *p;
         }
      }
      throw std::invalid_argument("JsonValue::get<NonNumeric> variant not satisfied.");
   }

   template <libjson::concepts::NonNumeric T> T &get() {
      if constexpr (std::is_same_v<T, JsonArray>) {
         if (auto p = std::get_if<std::shared_ptr<JsonArray>>(&_value)) {
            return **p;
         }
      } else if constexpr (std::is_same_v<T, JsonObject>) {
         if (auto p = std::get_if<std::shared_ptr<JsonObject>>(&_value)) {
            return **p;
         }
      } else {
         if (auto p = std::get_if<T>(&_value)) {
            return *p;
         }
      }
      throw std::invalid_argument("JsonValue::get<NonNumeric> variant not satisfied.");
   }

 private:
   JsonValueVariantType _value;
   ValueType            _type;
};

//--------------------
// collection types
// - object
// - array
//--------------------

// ---------
// - array -
// ---------
using JsonArrayData = std::vector<JsonValue>;
class JsonArray {
 public:
   JsonArray() = default;
   JsonArray(const JsonArrayData &data) : _data(data) {};

   auto begin() {
      return _data.begin();
   }
   auto end() {
      return _data.end();
   }
   const auto cbegin() const {
      return _data.cbegin();
   }
   const auto cend() const {
      return _data.cend();
   }
   size_t size() {
      return _data.size();
   }
   JsonValue operator[](size_t offset) const {
      return _data[offset];
   }

   template <libjson::concepts::Numeric T> T get(size_t index) const {
      return _data.at(index).get<T>();
   }

   template <libjson::concepts::NonNumeric T> T get(size_t index) const {
      return _data.at(index).get<T>();
   }

   template <libjson::concepts::NonNumeric T> T &get(size_t index) {
      return _data.at(index).get<T>();
   }

 private:
   JsonArrayData _data;
};

// ----------
// - object -
// ----------
using JsonObjectData = std::unordered_map<std::string, JsonValue>;
class JsonObject {
 public:
   JsonObject() = default;
   JsonObject(const JsonObjectData &data) : _data(data) {};
   JsonObject(std::initializer_list<std::pair<std::string, JsonValue>> list)
       : _data(list.begin(), list.end()) {}

   auto begin() {
      return _data.begin();
   }
   auto end() {
      return _data.end();
   }
   const auto cbegin() const {
      return _data.cbegin();
   }
   const auto cend() const {
      return _data.cend();
   }
   size_t size() {
      return _data.size();
   }
   JsonValue operator[](const std::string &key) {
      return _data[key];
   }

   template <libjson::concepts::Numeric T> T get(const std::string &key) const {
      return _data.at(key).get<T>();
   }

   template <libjson::concepts::NonNumeric T> const T get(const std::string &key) const {
      if constexpr (std::is_same_v<T, JsonValue>) {
         return _data.at(key);
      } else {
         return _data.at(key).get<T>();
      }
   }

   template <libjson::concepts::NonNumeric T> const T &get(const std::string &key) {
      if constexpr (std::is_same_v<T, JsonValue>) {
         return _data.at(key);
      } else {
         return _data.at(key).get<T>();
      }
   }

   bool has(const std::string &key) const {
      return _data.contains(key);
   }

 private:
   JsonObjectData _data;
};

} // namespace libjson
