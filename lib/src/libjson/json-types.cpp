#include "json-types.h"

namespace libjson {

JsonValue::JsonValue(JsonArray &&array)
    : _value(std::make_unique<JsonArray>(std::move(array))), _type(ValueType::ARRAY) {}

JsonValue::JsonValue(JsonObject &&object)
    : _value(std::make_unique<JsonObject>(std::move(object))), _type(ValueType::OBJECT) {}

JsonArray::JsonArray(JsonArrayData &&data) : _data(std::move(data)) {};
} // namespace libjson
