#pragma once
#include <string>

namespace libjson {
class JSONNumber {
public:
  JSONNumber(const std::string &literal) : _literal(literal) {}

  template <typename T> T get();

private:
  std::string _literal;
};

template <> inline int JSONNumber::get<int>() { return std::stoi(_literal); }
template <> inline long JSONNumber::get<long>() { return std::stol(_literal); }
template <> inline long long JSONNumber::get<long long>() {
  return std::stoll(_literal);
}
template <> inline unsigned long JSONNumber::get<unsigned long>() {
  return std::stoul(_literal);
}
template <> inline unsigned long long JSONNumber::get<unsigned long long>() {
  return std::stoull(_literal);
}
template <> inline float JSONNumber::get<float>() {
  return std::stof(_literal);
}
template <> inline double JSONNumber::get<double>() {
  return std::stod(_literal);
}
template <> inline long double JSONNumber::get<long double>() {
  return std::stold(_literal);
}

} // namespace libjson
