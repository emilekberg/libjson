#pragma once

namespace libjson {
enum class TokenTypes {
   HEAD,
   CURLY_BRACE_OPEN,
   CURLY_BRACE_CLOSE,
   SQUARE_BRACKET_OPEN,
   SQUARE_BRACKET_CLOSE,
   COMMA,
   COLON,
   STRING,
   NUMBER,
   LITERAL,
   END_OF_FILE,
   END,
   ILLEGAL,
};
}
