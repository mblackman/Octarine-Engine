# Events

The engine has a small, type-erased publish/subscribe bus for decoupled communication — input,
collisions, and audio triggers flow through it rather than through direct calls between systems.

## The bus

`EventBus` (`src/EventBus/EventBus.h`) keys subscriptions by `std::type_index`, so any C++ type can be
an event; there's no base-class requirement beyond deriving from the empty `Event` marker
(`src/EventBus/Event.h`). The single bus instance lives in the engine's `EngineContext` singleton
(`src/Engine/EngineContext.h`), reachable as `registry->Get<EngineContext>().eventBus`. Systems
usually receive it in their `Init` / `SubscribeToEvents` hook rather than fetching it themselves.

API:

- `SubscribeEvent<Owner, Event>(owner, &Owner::method)` — register a member-function handler and
  return a `[[nodiscard]] EventBus::SubscriptionHandle`. The handler takes `Event&` or `const Event&`
  (both overloads exist). **You must keep the returned handle** — its destructor unsubscribes, so a
  discarded handle unsubscribes immediately.
- `EmitEvent<Event>(args...)` — construct an `Event` from `args` and dispatch it to every subscriber,
  synchronously, in subscription order. Emitting an event with no subscribers is a no-op.

```cpp
// Subscribe (typically in a system's Init/SubscribeToEvents) — store the handle as a member:
subscription_ = eventBus->SubscribeEvent<DamageSystem, CollisionEvent>(this, &DamageSystem::OnCollision);
// ...where the class declares:  EventBus::SubscriptionHandle subscription_;
// (use a std::vector<EventBus::SubscriptionHandle> when a system subscribes to several events).

// Emit (from wherever the thing happens):
eventBus->EmitEvent<CollisionEvent>(entityA, entityB);
```

Subscriptions are wired once during startup (`Game::Setup`, each system's `Init` /
`SubscribeToEvents`) and last as long as the handle the subscriber holds.

> **Lifetime is RAII, order-independent.** `SubscriptionHandle` owns the subscription: its destructor
> removes the callback, and it holds a `weak_ptr` to the bus's dispatch table, so destroying the handle
> *after* the bus is a safe no-op. A subscriber simply keeps its handle(s) as a member and never has to
> unsubscribe by hand — there is no `UnsubscribeAll` and no dangling-callback hazard. Call
> `handle.Reset()` to unsubscribe early; `handle.Active()` reports whether the subscription is still
> live on a surviving bus.

## Event catalog

| Event | Payload | Emitted by | Subscribed by |
|-------|---------|------------|---------------|
| `AudioPlayEvent` | `clipId: string`, `volume: float` | Lua `play_sound()` (`AudioModuleLuaBinding.cpp`) | `AudioSystem::OnAudioPlay` |
| `CollisionEvent` | `entityA: Entity`, `entityB: Entity` | `CollisionSystem` (narrowphase) | `DamageSystem::OnCollision`, `ObstacleBounceSystem::OnCollision` |
| `KeyInputEvent` | `inputKey: SDL_Keycode`, `inputModifier: SDL_Keymod`, `isPressed: bool` | `Game::ProcessInput` (SDL key events) | `InputSystem::OnKeyInput`, `FrameLoop::OnKeyInputEvent` |
| `MouseInputEvent` | `event: SDL_MouseButtonEvent` | `Game::ProcessInput` (SDL mouse-button events) | `InputSystem::OnMouseInput`, `UIButtonSystem::OnMouseInput` |
| `MouseWheelEvent` | `dx: float`, `dy: float` | `Game::ProcessInput` (SDL wheel events) | `InputSystem::OnMouseWheel` |

Event types live in `src/Events/`.

### Flows at a glance

- **Input:** SDL events → `Game::ProcessInput` emits `KeyInputEvent` / `MouseInputEvent` /
  `MouseWheelEvent` → `InputSystem` folds them into per-frame state (also `FrameLoop` for engine
  hotkeys, `UIButtonSystem` for clicks).
- **Collision:** `CollisionSystem` detects overlaps and emits `CollisionEvent` → `DamageSystem` and
  `ObstacleBounceSystem` react.
- **Audio:** Lua `play_sound(clip, volume)` emits `AudioPlayEvent` → `AudioSystem` plays the clip.

## Adding an event

Add `src/Events/<Name>Event.h` (derive from `Event`, give it a constructor over its payload). Emit it
with `EmitEvent<Name>(...)` at the producing site, and subscribe in each consumer's `Init` /
`SubscribeToEvents`. No registration list to update — the bus discovers types on first use.
