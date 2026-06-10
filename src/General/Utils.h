#pragma once
#include <numeric>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

inline std::vector<std::string> SplitString(const std::string& str, const char delimiter) {
  std::vector<std::string> tokens;
  std::istringstream stream(str);
  std::string token;

  while (std::getline(stream, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}

template <typename T>
bool HasFlag(T value, T flag) {
  using U = std::underlying_type_t<T>;
  return (static_cast<U>(value) & static_cast<U>(flag)) == static_cast<U>(flag);
}

inline float GetAverage(const std::vector<float>& vec) {
  if (vec.empty()) {
    return 0.0f;
  }

  const float sum = std::accumulate(vec.begin(), vec.end(), 0.0f);
  return sum / static_cast<float>(vec.size());
}
