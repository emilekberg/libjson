# libjson

[![CMake Build & Test](https://github.com/emilekberg/libjson/actions/workflows/cmake-build.yml/badge.svg)](https://github.com/emilekberg/libjson/actions/workflows/cmake-build.yml)

This is probably not a :fire: blazing fast parser. But I made it. With :hearts:

The purpose of this was for me to try writing a lexer and parser, rather than solving a non-existing problem.

Lexer expects an `std::istream`. Lexer parses lazy and returns a `Token_EndOfFile` when reaching end of file, and beyond that will return a `Token_End`.

It evaluates numbers from strings when you requrest it so take note of that. This is tradeoff for sake of supporting more arithmetic types.

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
lexerdata/large-file.json   64530724 ns     66304943 ns           10
data/large-file.json       167962598 ns    172580949 ns            4
lexerdata/00.json           27831800 ns     28597147 ns           24
data/00.json                88750823 ns     91192360 ns            7
JsonGetNumber_                  28.0 ns         28.7 ns     24506028
JsonGetObject_                   109 ns          112 ns      6333009
```

## Todo

- [/] https://github.com/briandfoy/json-acceptance-tests/tree/master
  - supports most, some which i've intentionally left out for now.
- [ ] support both `std::string_view` and `std::istream`.
  - I expect `std::string_view` to be faster, but require more memory ahead of time.

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

