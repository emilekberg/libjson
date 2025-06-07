#pragma once

namespace libjson {
enum class TokenTypes {
  ILLEGAL,
  SEPARATOR,   // {}[],:
  STRING,      // "someString" or "you can \"escape\" values" or 'single fnutts'
  NUMBER,      // 3.14 or 5 or -123
  LITERAL,     // true, false, null
  END_OF_FILE, // We're done!
};
}
