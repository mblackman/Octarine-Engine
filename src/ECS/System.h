#pragma once

#include <string>
#include <string_view>
#include <utility>

class Registry;

class ISystem {
 public:
  virtual ~ISystem() = default;
  virtual void Update(const Registry& registry) = 0;

  [[nodiscard]] const std::string& GetName() const { return name_; }

 protected:
  explicit ISystem(std::string name) : name_(std::move(name)) {}

  // Trim MSVC's "class " / "struct " prefix from typeid::name() so profiler output reads
  // as e.g. `MovementSystem` rather than `class MovementSystem`.
  static std::string PrettifyTypeName(const char* raw) {
    std::string_view sv(raw);
    constexpr std::string_view kClass = "class ";
    constexpr std::string_view kStruct = "struct ";
    if (sv.starts_with(kClass))
      sv.remove_prefix(kClass.size());
    else if (sv.starts_with(kStruct))
      sv.remove_prefix(kStruct.size());

    // Trim leading numbers (GCC/Clang typeid(T).name() often starts with length)
    size_t firstNonDigit = 0;
    while (firstNonDigit < sv.size() && std::isdigit(static_cast<unsigned char>(sv[firstNonDigit]))) {
      firstNonDigit++;
    }
    if (firstNonDigit > 0) {
      sv.remove_prefix(firstNonDigit);
    }

    return std::string(sv);
  }

 private:
  std::string name_;
};