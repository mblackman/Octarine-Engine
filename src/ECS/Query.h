#pragma once
#include <algorithm>

#include "../General/PerfUtils.h"
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
    RebuildSorted();
  }

  void Update() override {
    ACCUMULATE_PROFILE_SCOPE("Query::Update");
    const uint64_t current_gen = registry_->ArchetypeGeneration();
    if (current_gen == cached_generation_) {
      return;  // Archetype set unchanged — skip the re-match.
    }
    ACCUMULATE_PROFILE_SCOPE("Query::Update (rematch)");
    cached_generation_ = current_gen;
    auto matching_archetypes = registry_->GetMatchingArchetypes(sorted_type_);

    if (!excluded_.empty()) {
      std::erase_if(matching_archetypes, [&](Archetype* arch) {
        for (const ComponentID id : excluded_) {
          if (arch->HasComponent(id)) return true;
        }
        return false;
      });
    }

    archetype_query_ = ArchetypeQuery<TComponents...>(type_, matching_archetypes, include_inactive_);
  }

  // Add a tag/label as a query filter. Filtered tags are required for archetype matching but are
  // not yielded to ForEach — type_ (template-pack order) stays untouched, so Iterator's mapping
  // from TComponents...[Is] to type_[Is] remains valid.
  ComponentQuery& WithTag(const Entity tag) {
    extra_required_.push_back(tag.GetId());
    RebuildSorted();
    cached_generation_ = UINT64_MAX;  // Force re-match on next Update.
    return *this;
  }

  ComponentQuery& WithTag(const std::string& name) { return WithTag(registry_->TagId(name)); }

  ComponentQuery& WithoutTag(const Entity tag) {
    excluded_.push_back(tag.GetId());
    cached_generation_ = UINT64_MAX;
    return *this;
  }

  ComponentQuery& WithoutTag(const std::string& name) { return WithoutTag(registry_->TagId(name)); }

  // Typed tag filters — resolve through Registry::Tag<T>() for an array-indexed lookup rather
  // than a string hash. Same semantics as WithTag/WithoutTag; use these for engine-internal tags.
  template <typename T>
  ComponentQuery& With() {
    return WithTag(registry_->template Tag<T>());
  }

  template <typename T>
  ComponentQuery& Without() {
    return WithoutTag(registry_->template Tag<T>());
  }

  // Opt-in to iterating parked/inactive entities. Default queries see only the active prefix
  // of each chunk; this is the escape hatch for diagnostics/pool internals that explicitly
  // want both regions. Forces a re-build of the underlying ArchetypeQuery on next Update().
  ComponentQuery& IncludeInactive() {
    include_inactive_ = true;
    cached_generation_ = UINT64_MAX;
    return *this;
  }

  template <typename Func>
  void ForEach(Func&& func) {
    if constexpr (std::is_invocable_v<Func, ContextFacade&, Entity, TComponents&...> ||
                  std::is_invocable_v<Func, ContextFacade&, TComponents&...>) {
      for (const auto iterable = CreateIterable(); auto&& context : iterable) {
        if constexpr (std::is_invocable_v<Func, ContextFacade&, Entity, TComponents&...>) {
          func(context, context.GetEntity(), context.template Component<TComponents>()...);
        } else {
          func(context, context.template Component<TComponents>()...);
        }
      }
    } else {
      for (auto it = archetype_query_.begin(); it != archetype_query_.end(); ++it) {
        std::apply(
            [&](Entity e, TComponents&... comps) {
              if constexpr (std::is_invocable_v<Func, Entity, TComponents&...>) {
                func(e, comps...);
              } else if constexpr (std::is_invocable_v<Func, TComponents&...>) {
                func(comps...);
              } else {
                static_assert(!std::is_same_v<Func, Func>,
                              "The function passed to ForEach does not match the required signatures. "
                              "Expected one of: void(ContextFacade&, Entity, T&...), void(ContextFacade&, T&...), "
                              "void(Entity, T&...), or void(T&...).");
              }
            },
            *it);
      }
    }
  }

  // Parallel version of ForEach — distributes chunks across CPU threads.
  // Only safe for funcs that do per-entity independent writes (no shared mutable state).
  // Func signature: void (Entity, TComponents&...) or void(TComponents&...).
  template <typename Func>
  void ParallelForEach(Func&& func) {
    archetype_query_.ParallelForEach(std::forward<Func>(func));
  }

  template <typename Func>
  void Iterate(Func&& func) {
    func(CreateIterable());
  }

 private:
  void RebuildSorted() {
    sorted_type_ = type_;
    for (const ComponentID id : extra_required_) {
      if (std::ranges::find(sorted_type_, id) == sorted_type_.end()) {
        sorted_type_.push_back(id);
      }
    }
    std::ranges::sort(sorted_type_);
  }

  Iterable CreateIterable() {
    auto beginFunc = [&] {
      return AnyIterator(std::make_unique<Internal::IteratorImpl<TComponents...>>(archetype_query_.begin(), registry_,
                                                                                  registry_->delta_time_));
    };
    auto endFunc = [&] {
      return AnyIterator(std::make_unique<Internal::IteratorImpl<TComponents...>>(archetype_query_.end(), registry_,
                                                                                  registry_->delta_time_));
    };

    return Iterable(beginFunc, endFunc);
  }

  Registry* registry_;
  ArchetypeType type_;            // user-pack order — used by ArchetypeQuery::Iterator
  ArchetypeType sorted_type_;     // sorted ascending — used for archetype matching
  ArchetypeType extra_required_;  // additional ComponentIDs required (e.g., tag filters)
  ArchetypeType excluded_;        // ComponentIDs that disqualify an archetype
  ArchetypeQuery<TComponents...> archetype_query_;
  uint64_t cached_generation_{UINT64_MAX};  // Forces first Update to always match.
  bool include_inactive_ = false;
};
