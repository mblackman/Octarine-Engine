#pragma once

#include <cassert>
#include <memory>
#include <type_traits>

#include "../General/Logger.h"
#include "Entity.h"

class Registry;

class AnyContext {
 public:
  AnyContext() = default;
  virtual ~AnyContext() = default;
  AnyContext(const AnyContext&) = delete;
  AnyContext& operator=(const AnyContext&) = delete;

  AnyContext(AnyContext&&) = delete;
  AnyContext& operator=(AnyContext&&) = delete;

  [[nodiscard]] virtual Entity GetEntity() const = 0;
  [[nodiscard]] virtual Registry* GetRegistry() const = 0;
  [[nodiscard]] virtual float GetDeltaTime() const = 0;
  [[nodiscard]] virtual void* GetComponentPtr(EntityID id) = 0;
};

class ContextFacade {
 public:
  explicit ContextFacade(AnyContext* impl) : impl_(impl) {}

  [[nodiscard]] Entity GetEntity() const { return impl_->GetEntity(); }
  [[nodiscard]] Registry* GetRegistry() const { return impl_->GetRegistry(); }
  [[nodiscard]] float GetDeltaTime() const { return impl_->GetDeltaTime(); }

  // Per-entity component access. Only valid inside a per-entity (RegisterSystem) callback.
  // RegisterBulkSystem callbacks receive a sentinel context — they must iterate the Iterable
  // and read components off the inner ContextFacade. Calling Component<T>() on a bulk
  // sentinel returns nullptr and trips an assertion in BulkContextImpl::GetComponentPtr.
  template <typename T>
  T& Component() const;

  template <typename T>
  T& Component(const ComponentID id) const {
    void* ptr = impl_->GetComponentPtr(id);
    return *static_cast<T*>(ptr);
  }

 private:
  AnyContext* impl_;
};

namespace Internal {

class BulkContextImpl final : public AnyContext {
 public:
  BulkContextImpl(Registry* registry, const float dt) : registry_(registry), dt_(dt) {}

  [[nodiscard]] Entity GetEntity() const override { return Entity{}; }
  [[nodiscard]] Registry* GetRegistry() const override { return registry_; }
  [[nodiscard]] float GetDeltaTime() const override { return dt_; }
  void* GetComponentPtr(EntityID /*id*/) override {
    Logger::Error(
        "ContextFacade::Component<T>() called on a bulk-system context. Bulk callbacks must "
        "iterate the Iterable and access components on the inner per-entity context.");
    assert(false && "ContextFacade::Component<T>() not valid in a bulk system");
    return nullptr;
  }

 private:
  Registry* registry_;
  float dt_;
};

}  // namespace Internal
