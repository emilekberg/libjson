#pragma once
#include "object.h"
#include "token.h"
#include <string>
#include <vector>
namespace libjson {
Object parse(const std::string &input);
std::vector<Token> tokenize(const std::string &input);
} // namespace libjson
