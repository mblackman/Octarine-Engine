#pragma once
#include "Iterable.h"
#include "Registry.h"

class Query {
 public:
  Query() = default;
  virtual ~Query() = default;

  Query(Query&&) = default;
  Query(Query& query) = default;

  Query& operator=(Query&&) = default;
  Query& operator=(const Query& query) = default;

  virtual void Update() = 0;
};

template <typename... TComponents>
class ComponentQuery final : public Query {
 public:
  explicit ComponentQuery(Registry* registry)
      : registry_(registry), type_({(registry->Component<TComponents>().id)...}) {}

  void Update() override {
    const auto matching_archetypes = registry_->GetMatchingArchetypes(type_);
    archetype_query_ = ArchetypeQuery<TComponents...>(type_, matching_archetypes);
  }

  template <typename Func>
  void ForEach(Func func) {
    for (const auto iterable = CreateIterable(); auto&& context : iterable) {
      if constexpr (std::is_invocable_v<Func, ContextFacade&, TComponents&...>) {
        func(context, context.Component<TComponents>()...);
      } else if constexpr (std::is_invocable_v<Func, Entity, TComponents&...>) {
        func(context.Entity(), context.Entity(), context.Component<TComponents>()...);
      } else if constexpr (std::is_invocable_v<Func, TComponents&...>) {
        func(context.Component<TComponents>()...);
      } else {
        static_assert(!std::is_same_v<Func, Func>,
                      "The function passed to ForEach does not match the required signatures. "
                      "Expected one of: void(Iter&, T&...), void(Entity, T&...), or void(T&...).");
      }
    }
  }

  template <typename Func>
  void Iterate(Func&& func) {
    func(CreateIterable());
  }

 private:
  Iterable CreateIterable() {
    auto beginFunc = [&]() {
      return AnyIterator(std::make_unique<Internal::IteratorImpl<TComponents...>>(archetype_query_.begin(), registry_,
                                                                                  registry_->delta_time_));
    };
    auto endFunc = [&]() {
      return AnyIterator(std::make_unique<Internal::IteratorImpl<TComponents...>>(archetype_query_.end(), registry_,
                                                                                  registry_->delta_time_));
    };

    return Iterable(beginFunc, endFunc);
  }

  Registry* registry_;
  ArchetypeType type_;
  ArchetypeQuery<TComponents...> archetype_query_;
};
