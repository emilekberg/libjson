# libjson

[![CMake Build & Test](https://github.com/emilekberg/libjson/actions/workflows/cmake-build.yml/badge.svg)](https://github.com/emilekberg/libjson/actions/workflows/cmake-build.yml)

This is probably not a :fire: blazing fast parser. But I made it. With :hearts:

The purpose of this was for me to try writing a lexer and parser, rather than solving a non-existing problem.

Lexer expects an `std::istream`. Lexer parses lazy and returns a `Token_EndOfFile` when reaching end of file, and beyond that will return a `Token_End`.

Values are stored as: 
| libjson     | std                                          |
|-------------|----------------------------------------------|
| JsonString  | std::string                                  |
| JsonNumber  | std::int64_t,double                          |
| JsonArray   |  std::vector<JsonValue>                      |
| JsonObject  | std::unoredered_map<std::string, JsonValue>  |
| JsonBool    | bool                                         |
| JsonNull    | nullptr, nullptr_t                           |


## How to 

### Clone

To run tests you need the submodules. Either clone by using 
```bash
git clone --recurse-submodules --remote-submodules
```

or sync them later by doing

```bash
git submodule init
git submodule update
```

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
./build/Release/benchmark/libjson_bench
```

```
--------------------------------------------------------------------
Benchmark                          Time             CPU   Iterations
--------------------------------------------------------------------
lexerdata/large-file.json       66.4 ms         64.6 ms           10
data/large-file.json             173 ms          169 ms            4
lexerdata/00.json               29.2 ms         28.4 ms           25
data/00.json                    92.5 ms         90.1 ms            8
JsonGetNumber_                 0.000 ms        0.000 ms     24703439
JsonGetObject_                 0.000 ms        0.000 ms      6396276
```

## Todo

- [/] https://github.com/briandfoy/json-acceptance-tests/tree/master
  - supports most, some which i've intentionally left out for now.
- [ ] support both `std::string_view` and `std::istream`.
  - I expect `std::string_view` to be faster, but require more memory ahead of time.

### Caveats

- Does not support number exponents yet
- Does not support serializing to string.

## Example

### Construction
```cpp
libjson::JsonObject obj({
  {"name", "emil"},
  {"age", JsonNumber("12")},
});

std::cout << "name is: " << obj.get<JsonString>("name") << std::endl;
std::cout << "age is: " << obj.get<uint16_t>("age") << std::endl;
```


### Parse
```cpp
std::istringstream stream(R"({"key":"value"})");
libjson::JsonObject result = libjson::parse(stream);
if (result.has("key"))
{
    std::cout << result.get<std::string>("key") << std::endl;
}
```

source: https://www.json.org/json-en.html

