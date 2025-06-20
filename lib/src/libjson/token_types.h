#pragma once

namespace libjson {
enum class TokenTypes {
  HEAD, // used for beginning, maybe
  // SEPARATOR, // {}[],:
  LEFT_BRACE,
  RIGHT_BRACE,
  LEFT_BRACKET,
  RIGHT_BRACKET,
  COMMA,
  COLON,
  STRING,      // "someString" or "you can \"escape\" values" or 'single fnutts'
  NUMBER,      // 3.14 or 5 or -123
  LITERAL,     // true, false, null
  END_OF_FILE, // We're done!
  END,         // used when iterating to identify end
  ILLEGAL,     // unwanted type.
};
}
