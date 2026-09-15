#pragma once

#include <cstddef>
#include <string>
#include <string_view>
#include <utility>

class Registry;

// Index of a registered system in Registry.
using SystemId = std::size_t;

// Handle to a registered system providing system ID and functor access.
template <typename StoredFunc>
class SystemHandle {
 public:
  SystemHandle(const SystemId id, StoredFunc* func) : id_(id), func_(func) {}

  [[nodiscard]] SystemId Id() const { return id_; }
  [[nodiscard]] StoredFunc& Func() const { return *func_; }

 private:
  SystemId id_;
  StoredFunc* func_;
};

class ISystem {
 public:
  virtual ~ISystem() = default;
  virtual void Update(const Registry& registry) = 0;

  [[nodiscard]] const std::string& GetName() const { return name_; }

 protected:
  explicit ISystem(std::string name) : name_(std::move(name)) {}

  // Strips compiler typeid prefixes and mangling for profiler labels.
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