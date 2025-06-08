# libjson

[![CMake Build & Test](https://github.com/emilekberg/libjson/actions/workflows/cmake-build.yml/badge.svg)](https://github.com/emilekberg/libjson/actions/workflows/cmake-build.yml)

This is probably not a :fire: blazing fast parser. But I made it. With :hearts:

The purpose of this was for me to try writing a lexer and parser, rather than solving a non-existing problem.

It uses `std::string_view` internally so make sure your string outlives the parsing scope. Once you have a json object it should be all good however as it copies strings and values into the json data where it needs (strings, numbers).

It evaluates numbers from strings when you requrest it so take note of that. This is tradeoff for sake of supporting more arithmetic types.

## How to 

### Build

```bash
# debug
cmake -S . -B build/Debug -G Ninja
cmake --build build/Debug

# release
cmake -DCMAKE_BUILD_TYPE=Release -S . -B build/Release -G Ninja
cmake --build build/Release
```

### Run Benchmark

build `release` like above, then
```bash
./build/Debug/benchmark/libjson_bench
```

## Todo

- [/] https://github.com/briandfoy/json-acceptance-tests/tree/master
  - supports most, some which i've intentionally left out for now.

## Features

- Arrays
- Objects
- Strings
- Literals
  - bool, both true/false
  - null
- Numbers
  - Lazy evaluated, might not be the fastest, but takes less memory and supports all (i think) arithmeitc types. 

### Caveats

- Does not support number exponents yet
- Does not support serializing to string.

## Example

```cpp
std::string json = R"({"key":"value"})";
libjson::JSONObject result = libjson::parse(json);
if (result.has("key"))
{
    std::cout << result.get<std::string>("key") << std::endl;
}
```

source: https://www.json.org/json-en.html

