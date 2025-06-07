# libjson

This is probably not a :fire: blazing fast parser. But I made it. With :hearts:

## Features

- Arrays
- Objects
- Strings
- Literals
  - bool (true/false)
  - null (sort of)
- Numbers
  - as long as it's decimal :) 

## Example

```cpp
libjson::JSONObject result = libjson::parse(R"({"key":"value"})");
if (result.has("key"))
{
    std::cout << result.get<std::string>("key") << std::endl;
}
```

https://www.json.org/json-en.html
