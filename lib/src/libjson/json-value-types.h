#pragma once
namespace libjson {
enum class JsonValueType {
  __ERROR__,
  STRING,
  NUMBER,
  OBJECT,
  ARRAY,
  BOOL,
  _NULL_,
};
}
