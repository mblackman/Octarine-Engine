#pragma once

#include "ArchetypeQuery.h"
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
  [[nodiscard]] virtual void* GetComponentPtr(ComponentID id) = 0;
};

class ContextFacade {
 public:
  explicit ContextFacade(std::unique_ptr<AnyContext> impl) : impl_(std::move(impl)) {}

  [[nodiscard]] Entity Entity() const { return impl_->GetEntity(); }
  [[nodiscard]] Registry* Registry() const { return impl_->GetRegistry(); }
  [[nodiscard]] float DeltaTime() const { return impl_->GetDeltaTime(); }

  template <typename T>
  T& Component() const {
    void* ptr = impl_->GetComponentPtr(GetComponentID<T>());
    return *static_cast<T*>(ptr);
  }

 private:
  std::unique_ptr<AnyContext> impl_;
};

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
  [[nodiscard]] virtual std::unique_ptr<AnyContext> Dereference() const = 0;
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
void* FindComponentInTuple(ComponentID id, std::tuple<TComponents&...>& tuple) {
  void* ptr = nullptr;
  auto check = [&]<typename T>(T& component) {
    if (GetComponentID<std::remove_reference_t<T>>() == id) {
      ptr = &component;
    }
  };
  (check(std::get<TComponents&>(tuple)), ...);
  return ptr;
}

template <typename... TComponents>
class ContextImpl final : public AnyContext {
 public:
  ContextImpl(Registry* registry, const float dt, const Entity entity, std::tuple<TComponents&...> components)
      : registry_(registry), dt_(dt), entity_(entity), components_(components) {}

  [[nodiscard]] Entity GetEntity() const override { return entity_; }
  [[nodiscard]] Registry* GetRegistry() const override { return registry_; }
  [[nodiscard]] float GetDeltaTime() const override { return dt_; }

  void* GetComponentPtr(ComponentID id) override { return FindComponentInTuple<TComponents...>(id, components_); }

 private:
  Registry* registry_;
  float dt_;
  Entity entity_;
  std::tuple<TComponents&...> components_;
};

template <typename... TComponents>
class IteratorImpl final : public AnyIteratorImpl {
  using UnderlyingIterator = typename ArchetypeQuery<TComponents...>::Iterator;

 public:
  IteratorImpl(UnderlyingIterator it, Registry* registry, const float dt) : it_(it), registry_(registry), dt_(dt) {}

  void Increment() override { ++it_; }

  [[nodiscard]] bool IsEqual(const AnyIteratorImpl& other) const override {
    const auto& other_impl = static_cast<const IteratorImpl&>(other);
    return it_ == other_impl.it_;
  }

  [[nodiscard]] std::unique_ptr<AnyContext> Dereference() const override {
    return std::apply(
        [&](Entity e, TComponents&... comps) {
          return std::make_unique<ContextImpl<TComponents...>>(registry_, dt_, e, std::tie(comps...));
        },
        *it_);
  }

  [[nodiscard]] std::unique_ptr<AnyIteratorImpl> Clone() const override {
    return std::make_unique<IteratorImpl>(*this);
  }

 private:
  UnderlyingIterator it_;
  Registry* registry_;
  float dt_;
};

}  // namespace Internal