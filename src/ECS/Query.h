#pragma once
#include <algorithm>

#include "General/PerfUtils.h"
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

template <typename T>
struct Opt {};  // marker only — never instantiated

template <typename... TComponents>
class ComponentQuery final : public Query {
 public:
  explicit ComponentQuery(Registry* registry)
      : registry_(registry), type_({(registry->Component<Internal::unwrap_opt_t<TComponents>>().GetId())...}) {
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

    if (cached_generation_ == UINT64_MAX) {
      // First Update, or a filter changed (WithTag/WithoutTag/IncludeInactive): full rebuild.
      matched_ = registry_->GetMatchingArchetypes(sorted_type_);
      if (!excluded_.empty()) {
        std::erase_if(matched_, [&](Archetype* arch) { return IsExcluded(*arch); });
      }
    } else if (!sorted_type_.empty()) {
      // Incremental: archetypes are never destroyed, so the previous match list stays valid —
      // only archetypes created since the cached generation need testing. This keeps a burst of
      // new archetypes O(new) per query instead of a full re-scan of every archetype.
      const auto& log = registry_->ArchetypeLog();
      for (uint64_t gen = cached_generation_; gen < current_gen; ++gen) {
        Archetype* arch = log[gen];
        if (Registry::MatchesType(*arch, sorted_type_) && !IsExcluded(*arch)) {
          matched_.push_back(arch);
        }
      }
    }

    cached_generation_ = current_gen;
    archetype_query_ = ArchetypeQuery<TComponents...>(type_, matched_, include_inactive_);
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
      ForEachWithFacade(std::forward<Func>(func));
    } else {
      // end() is hoisted out of the condition — constructing an Iterator per loop pass is a
      // per-entity cost on the hot path.
      for (auto it = archetype_query_.begin(), endIt = archetype_query_.end(); it != endIt; ++it) {
        std::apply(
            [&](Entity e, auto&&... comps) {
              if constexpr (std::is_invocable_v<Func, Entity, decltype(comps)...>) {
                func(e, std::forward<decltype(comps)>(comps)...);
              } else if constexpr (std::is_invocable_v<Func, decltype(comps)...>) {
                func(std::forward<decltype(comps)>(comps)...);
              } else {
                static_assert(!std::is_same_v<Func, Func>,
                              "The function passed to ForEach does not match the required signatures. "
                              "Expected one of: void(ContextFacade&, Entity, T&...), void(ContextFacade&, T&...), "
                              "void(Entity, T&..., U*...), or void(T&..., U*...).");
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

  [[nodiscard]] size_t GetCount() const { return archetype_query_.GetTotalEntityCount(); }

 private:
  // ContextFacade-based iteration doesn't easily support optional components yet (requires
  // updating ContextFacade and IteratorImpl). For now, systems using Opt<T> must use the
  // simpler void(Entity, T&, U*...) or void(T&, U*...) signatures.
  template <typename Func>
  void ForEachWithFacade(Func&& func) {
    static_assert(!(... || Internal::is_optional_v<TComponents>),
                  "Optional components are not yet supported in ContextFacade-based ForEach.");
    for (const auto iterable = CreateIterable(); auto&& context : iterable) {
      if constexpr (std::is_invocable_v<Func, ContextFacade&, Entity, TComponents&...>) {
        func(context, context.GetEntity(), context.template Component<TComponents>()...);
      } else {
        func(context, context.template Component<TComponents>()...);
      }
    }
  }

  [[nodiscard]] bool IsExcluded(const Archetype& arch) const {
    return std::ranges::any_of(excluded_, [&](const ComponentID id) { return arch.HasComponent(id); });
  }

  void RebuildSorted() {
    sorted_type_.clear();
    // Only components NOT wrapped in Opt<T> drive matching.
    (
        [&] {
          if constexpr (!Internal::is_optional_v<TComponents>) {
            sorted_type_.push_back(registry_->Component<Internal::unwrap_opt_t<TComponents>>().GetId());
          }
        }(),
        ...);

    for (const ComponentID id : extra_required_) {
      if (std::ranges::find(sorted_type_, id) == sorted_type_.end()) {
        sorted_type_.push_back(id);
      }
    }
    std::ranges::sort(sorted_type_);
  }

  Iterable CreateIterable() {
    // CreateIterable currently only works for required components because IteratorImpl is not
    // yet optional-aware.
    return Iterable(
        [&] {
          return AnyIterator(std::make_unique<Internal::IteratorImpl<TComponents...>>(
              archetype_query_.begin(), registry_, registry_->DeltaTime()));
        },
        [&] {
          return AnyIterator(std::make_unique<Internal::IteratorImpl<TComponents...>>(archetype_query_.end(), registry_,
                                                                                      registry_->DeltaTime()));
        });
  }

  Registry* registry_;
  ArchetypeType type_;               // user-pack order — used by ArchetypeQuery::Iterator
  ArchetypeType sorted_type_;        // sorted ascending — used for archetype matching
  ArchetypeType extra_required_;     // additional ComponentIDs required (e.g., tag filters)
  ArchetypeType excluded_;           // ComponentIDs that disqualify an archetype
  std::vector<Archetype*> matched_;  // Persistent match list, appended to incrementally.
  ArchetypeQuery<TComponents...> archetype_query_;
  uint64_t cached_generation_{UINT64_MAX};  // Forces first Update to always match.
  bool include_inactive_ = false;
};
