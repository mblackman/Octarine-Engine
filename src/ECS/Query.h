#pragma once
#include <algorithm>

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
      : registry_(registry), type_({(registry->Component<TComponents>().GetId())...}) {
    // type_ stays in user-pack order so Iterator can map TComponents...[Is] to type_[Is].
    // sorted_type_ is the canonical form used for archetype matching (two-pointer superset check).
    sorted_type_ = type_;
    std::ranges::sort(sorted_type_);
  }

  void Update() override {
    const auto matching_archetypes = registry_->GetMatchingArchetypes(sorted_type_);
    archetype_query_ = ArchetypeQuery<TComponents...>(type_, matching_archetypes);
  }

  template <typename Func>
  void ForEach(Func& func) {
    for (const auto iterable = CreateIterable(); auto&& context : iterable) {
      if constexpr (std::is_invocable_v<Func, ContextFacade&, TComponents&...>) {
        func(context, context.Component<TComponents>()...);
      } else if constexpr (std::is_invocable_v<Func, Entity, TComponents&...>) {
        func(context.Entity(), context.Component<TComponents>()...);
      } else if constexpr (std::is_invocable_v<Func, TComponents&...>) {
        func(context.Component<TComponents>()...);
      } else {
        static_assert(!std::is_same_v<Func, Func>,
                      "The function passed to ForEach does not match the required signatures. "
                      "Expected one of: void(ContextFacade&, T&...), void(Entity, T&...), or void(T&...).");
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
  ArchetypeType type_;         // user-pack order — used by ArchetypeQuery::Iterator
  ArchetypeType sorted_type_;  // sorted ascending — used for archetype matching
  ArchetypeQuery<TComponents...> archetype_query_;
};
