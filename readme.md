# libjson

[![CMake Build & Test](https://github.com/emilekberg/libjson/actions/workflows/cmake-build.yml/badge.svg)](https://github.com/emilekberg/libjson/actions/workflows/cmake-build.yml)

This is probably not a :fire: blazing fast parser. But I made it. With :hearts:

The purpose of this was for me to try writing a lexer and parser, rather than solving a non-existing problem.

It uses `std::string_view` internally so make sure your string outlives the parsing scope. Once you have a json object it should be all good however as it copies strings and values into the json data where it needs (strings, numbers).

It evaluates numbers from strings when you requrest it so take note of that. This is tradeoff for sake of supporting more arithmetic types.

## Todo

- Add https://github.com/briandfoy/json-acceptance-tests/tree/master



## Benchmark

```
-------------------------------------------------------------------------
Benchmark                               Time             CPU   Iterations
-------------------------------------------------------------------------
MB_Parse_LargeJson             1896738562 ns   1898130165 ns            1
BM_ParseMtCars(32 rows)            102167 ns       102184 ns         6748
BM_ParseIris(150 rows)             206290 ns       206262 ns         3404
BM_ParseTitanic(891 rows)         2756810 ns      2755621 ns          255
BM_ParseHousePrice(1 460 rows)    1737837 ns      1736682 ns          403
BM_ParseWeather(2 922 rows)       1951446 ns      1949777 ns          358
```


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

