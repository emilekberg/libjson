#pragma once

namespace libjson {
enum class TokenTypes {
   HEAD, // used for beginning, maybe
   // SEPARATOR, // {}[],:
   CURLY_BRACE_OPEN,
   CURLY_BRACE_CLOSE,
   SQUARE_BRACKET_OPEN,
   SQUARE_BRACKET_CLOSE,
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
