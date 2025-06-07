#pragma once
#include "json-value.h"
#include <vector>
namespace libjson {
class JSONArray {
public:
  std::vector<JSONValue> data;
};

} // namespace libjson
