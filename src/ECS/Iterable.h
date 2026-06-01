#pragma once

#include <cassert>
#include <functional>
#include <memory>
#include <tuple>
#include <type_traits>

#include "../General/Logger.h"
#include "ArchetypeQuery.h"
#include "Context.h"
#include "Entity.h"
#include "Registry.h"

class Registry;

class AnyIteratorImpl {
 public:
  AnyIteratorImpl() = default;
  virtual ~AnyIteratorImpl() = default;
  AnyIteratorImpl(const AnyIteratorImpl&) = default;
  AnyIteratorImpl& operator=(const AnyIteratorImpl&) = default;
  AnyIteratorImpl(AnyIteratorImpl&&) = default;
  AnyIteratorImpl& operator=(AnyIteratorImpl&&) = default;

  virtual void Increment() = 0;
  [[nodiscard]] virtual bool IsEqual(const AnyIteratorImpl& other) const = 0;
  [[nodiscard]] virtual AnyContext* Dereference() const = 0;
  [[nodiscard]] virtual std::unique_ptr<AnyIteratorImpl> Clone() const = 0;
};

class AnyIterator {
 public:
  explicit AnyIterator(std::unique_ptr<AnyIteratorImpl> impl) : impl_(std::move(impl)) {}
  ~AnyIterator() = default;

  AnyIterator(const AnyIterator& other) : impl_(other.impl_->Clone()) {}
  AnyIterator& operator=(const AnyIterator& other) {
    if (this != &other) {
      impl_ = other.impl_->Clone();
    }
    return *this;
  }
  AnyIterator(AnyIterator&&) = default;
  AnyIterator& operator=(AnyIterator&&) = default;

  ContextFacade operator*() const { return ContextFacade(impl_->Dereference()); }

  AnyIterator& operator++() {
    impl_->Increment();
    return *this;
  }

  bool operator!=(const AnyIterator& other) const { return !impl_->IsEqual(*other.impl_); }

 private:
  std::unique_ptr<AnyIteratorImpl> impl_;
};

class Iterable {
 public:
  Iterable(std::function<AnyIterator()> begin, std::function<AnyIterator()> end)
      : begin_(std::move(begin)), end_(std::move(end)) {}

  [[nodiscard]] AnyIterator begin() const { return begin_(); }
  [[nodiscard]] AnyIterator end() const { return end_(); }

 private:
  std::function<AnyIterator()> begin_;
  std::function<AnyIterator()> end_;
};

namespace Internal {

template <typename... TComponents>
class ContextImpl final : public AnyContext {
 public:
  ContextImpl(Registry* registry, const float dt)
      : registry_(registry), dt_(dt), ids_{registry->Component<Internal::unwrap_opt_t<TComponents>>().GetId()...} {}

  void Update(const Entity entity, std::tuple<Internal::resolve_yield_t<TComponents>...> components) {
    entity_ = entity;
    components_ = std::apply(
        [](auto&&... comps) {
          return std::make_tuple([&]() {
            if constexpr (std::is_pointer_v<std::remove_reference_t<decltype(comps)>>) {
              return comps;
            } else {
              return &comps;
            }
          }()...);
        },
        components);
  }

  [[nodiscard]] Entity GetEntity() const override { return entity_; }
  [[nodiscard]] Registry* GetRegistry() const override { return registry_; }
  [[nodiscard]] float GetDeltaTime() const override { return dt_; }

  void* GetComponentPtr(EntityID id) override {
    void* ptr = nullptr;
    size_t i = 0;
    auto check = [&]<typename T0>(T0* component) {
      if (ptr) {
        return;
      }
      if (ids_[i] == id) {
        ptr = component;
      }
      ++i;
    };
    std::apply([&](auto... comps) { (check(comps), ...); }, components_);
    return ptr;
  }

 private:
  Registry* registry_;
  float dt_;
  Entity entity_{};
  std::tuple<Internal::unwrap_opt_t<TComponents>*...> components_;
  std::array<ComponentID, sizeof...(TComponents)> ids_;
};

template <typename... TComponents>
class IteratorImpl final : public AnyIteratorImpl {
  using UnderlyingIterator = typename ArchetypeQuery<TComponents...>::Iterator;

 public:
  IteratorImpl(UnderlyingIterator it, Registry* registry, const float dt)
      : it_(it), registry_(registry), dt_(dt), context_(registry, dt) {}

  IteratorImpl(const IteratorImpl& other)
      : AnyIteratorImpl(other),
        it_(other.it_),
        registry_(other.registry_),
        dt_(other.dt_),
        context_(other.registry_, other.dt_) {}

  void Increment() override { ++it_; }

  [[nodiscard]] bool IsEqual(const AnyIteratorImpl& other) const override {
    const auto& other_impl = static_cast<const IteratorImpl&>(other);
    return it_ == other_impl.it_;
  }

  [[nodiscard]] AnyContext* Dereference() const override {
    std::apply([&](Entity e, auto&&... comps) { context_.Update(e, std::forward_as_tuple(comps...)); }, *it_);
    return &context_;
  }

  [[nodiscard]] std::unique_ptr<AnyIteratorImpl> Clone() const override {
    return std::make_unique<IteratorImpl>(*this);
  }

 private:
  UnderlyingIterator it_;
  Registry* registry_;
  float dt_;
  mutable ContextImpl<TComponents...> context_;
};

}  // namespace Internal