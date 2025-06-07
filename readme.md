# libjson

This is probably not a :fire: blazing fast parser. But I made it. With :hearts:

## Example

```cpp
libjson::JSONObject result = libjson::parse(R"({"key":"value"})");
if (result.has("key"))
{
    std::cout << result.get<std::string>("key") << std::endl;
}
```

## How it works

- Starts with a Lexer that tokenizes the input
- Parses the tokanized string token by token, creating a object representation of the json string.

## Remarks

- numbers are only doubles atm. 
- not fast
- uses more memory than it should probably. 

https://www.json.org/json-en.html
