# libjson

[![CMake Build & Test](https://github.com/emilekberg/libjson/actions/workflows/cmake-build.yml/badge.svg)](https://github.com/emilekberg/libjson/actions/workflows/cmake-build.yml)

This is probably not a :fire: blazing fast parser. But I made it. With :hearts:

## Features

- Arrays
- Objects
- Strings
- Literals
  - bool, both true/false
  - null
- Numbers
  - Lazy evaluated, might not be the fastest, but takes less memory and supports all (i think) arithmeitc types. 

## Example

```cpp
libjson::JSONObject result = libjson::parse(R"({"key":"value"})");
if (result.has("key"))
{
    std::cout << result.get<std::string>("key") << std::endl;
}
```

https://www.json.org/json-en.html

