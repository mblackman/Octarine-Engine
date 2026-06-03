#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <benchmark/benchmark.h>

#include <cmath>
#include <cstdlib>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "Audio/AudioTrackCache.h"
#include "Components/AudioListenerComponent.h"
#include "Components/AudioSinkComponent.h"
#include "Components/AudioSourceComponent.h"
#include "Components/GlobalTransformComponent.h"
#include "Components/RigidBodyComponent.h"
#include "ECS/Iterable.h"
#include "ECS/Query.h"
#include "ECS/Registry.h"
#include "Systems/DopplerSystem.h"
#include "Systems/SpatialAudioSystem.h"

// Synthetic audio bench. Measures per-emitter cost of SpatialAudioSystem and DopplerSystem
// at steady state: emitters in range, geometry / velocity unchanged frame-over-frame, so
// the lastGain / lastPan / lastRatio dedup guards on AudioSinkComponent skip every MIX_Set*
// call. What we're left with is the math + pointer load + sentinel compare — the dominant
// cost when nothing is changing, which is the realistic 99% case.
//
// Transition-cost benches (in/out culling) are out of scope here; they'd profile SDL_mixer's
// internal state machine more than our systems.

namespace {
class StubContext final : public AnyContext {
 public:
  explicit StubContext(Registry* r) : registry_(r) {}
  [[nodiscard]] Entity GetEntity() const override { return Entity{}; }
  [[nodiscard]] Registry* GetRegistry() const override { return registry_; }
  [[nodiscard]] float GetDeltaTime() const override { return 0.016f; }
  void* GetComponentPtr(EntityID) override { return nullptr; }

 private:
  Registry* registry_;
};

// RAII helper: initializes SDL audio with the "dummy" driver + SDL_mixer once for the
// bench process. Skip the benchmark when init fails (CI containers without an audio
// device should still produce a clean skip rather than a crash).
struct AudioBenchEnv {
  AudioBenchEnv() {
    SDL_SetHint(SDL_HINT_AUDIO_DRIVER, "dummy");
    if (!SDL_Init(SDL_INIT_AUDIO) || !MIX_Init()) return;
    mixer_ = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
    initOk_ = mixer_ != nullptr;
  }

  ~AudioBenchEnv() {
    for (MIX_Track* t : tracks_) {
      if (t) MIX_DestroyTrack(t);
    }
    if (mixer_) MIX_DestroyMixer(mixer_);
    MIX_Quit();
    SDL_Quit();
  }

  AudioBenchEnv(const AudioBenchEnv&) = delete;
  AudioBenchEnv& operator=(const AudioBenchEnv&) = delete;

  [[nodiscard]] bool valid() const { return initOk_; }

  MIX_Track* AcquireTrack() {
    MIX_Track* t = MIX_CreateTrack(mixer_);
    if (t) tracks_.push_back(t);
    return t;
  }

 private:
  bool initOk_ = false;
  MIX_Mixer* mixer_ = nullptr;
  std::vector<MIX_Track*> tracks_;
};

struct BenchScene {
  Registry registry;
  std::unique_ptr<ComponentQuery<GlobalTransformComponent, AudioSourceComponent, AudioSinkComponent>> spatialQuery;
  std::unique_ptr<
      ComponentQuery<GlobalTransformComponent, RigidBodyComponent, AudioSourceComponent, AudioSinkComponent>>
      dopplerQuery;
  std::vector<Entity> emitters;
};

// Place N emitters in a square grid centered on the listener at (0,0). Spacing is sized
// so every emitter sits inside the 1000-unit maxDistance — i.e. on the attenuation +
// pan branch, the more expensive code path, rather than the early-out beyond maxDist.
void BuildSceneCommon(BenchScene& scene, AudioBenchEnv& env, int n, bool withRigidBody) {
  AudioListenerCache cache{};
  cache.valid = true;
  cache.position = glm::vec2(0.0f, 0.0f);
  cache.velocity = glm::vec2(0.0f, 0.0f);
  cache.right = glm::vec2(1.0f, 0.0f);
  cache.forward = glm::vec2(0.0f, 1.0f);
  cache.maxDistance = 1000.0f;
  cache.rolloff = 1.0f;
  cache.dopplerFactor = 1.0f;
  cache.speedOfSound = 343.0f;
  scene.registry.Set<AudioListenerCache>(cache);
  // The MIX_Track* lives in AudioTrackCache (keyed by entity), mirroring the engine; the
  // POD AudioSinkComponent only carries the per-frame dedup state the systems read/write.
  auto& trackCache = scene.registry.Set<AudioTrackCache>(AudioTrackCache());

  const int side = static_cast<int>(std::sqrt(static_cast<double>(n))) + 1;
  const float spacing = 800.0f / static_cast<float>(side);
  scene.emitters.reserve(static_cast<std::size_t>(n));
  int placed = 0;
  for (int row = 0; row < side && placed < n; ++row) {
    for (int col = 0; col < side && placed < n; ++col) {
      const float x = (static_cast<float>(col) - static_cast<float>(side) / 2.0f) * spacing;
      const float y = (static_cast<float>(row) - static_cast<float>(side) / 2.0f) * spacing;

      Entity e = scene.registry.CreateEntity();
      scene.registry.AddComponent(e, GlobalTransformComponent{glm::vec2(x, y), glm::vec2(1.0f, 1.0f), 0.0});
      scene.registry.AddComponent(
          e, AudioSourceComponent("synthetic", 0.8f, 1.0f, true, false, false, true, 50.0f, 1000.0f, true));
      if (withRigidBody) {
        scene.registry.AddComponent(e, RigidBodyComponent{glm::vec2(10.0f, 0.0f)});
      }
      MIX_Track* track = env.AcquireTrack();
      scene.registry.AddComponent(e, AudioSinkComponent{});
      trackCache.Store(e, track, 1);
      scene.emitters.push_back(e);
      ++placed;
    }
  }

  scene.spatialQuery = scene.registry.CreateQuery<GlobalTransformComponent, AudioSourceComponent, AudioSinkComponent>();
  scene.dopplerQuery =
      scene.registry
          .CreateQuery<GlobalTransformComponent, RigidBodyComponent, AudioSourceComponent, AudioSinkComponent>();
}
}  // namespace

static void BM_SpatialAudio_SteadyState(benchmark::State& state) {
  AudioBenchEnv env;
  if (!env.valid()) {
    state.SkipWithError("SDL3_mixer dummy-driver init failed");
    return;
  }

  BenchScene scene;
  BuildSceneCommon(scene, env, static_cast<int>(state.range(0)), /*withRigidBody=*/false);
  SpatialAudioSystem system;
  StubContext impl(&scene.registry);
  const ContextFacade ctx(&impl);

  // Warm-up frame populates sink.lastGain / lastPan / stereoApplied so the timed iters
  // exercise the dedup short-circuit (the realistic steady-state path).
  scene.spatialQuery->ForEach([&](const Entity e, const GlobalTransformComponent& tf, AudioSourceComponent& src,
                                  AudioSinkComponent& sink) { system(ctx, e, tf, src, sink); });

  for (auto _ : state) {
    scene.spatialQuery->ForEach([&](const Entity e, const GlobalTransformComponent& tf, AudioSourceComponent& src,
                                    AudioSinkComponent& sink) { system(ctx, e, tf, src, sink); });
  }
  state.SetItemsProcessed(static_cast<int64_t>(state.iterations()) * state.range(0));
}

BENCHMARK(BM_SpatialAudio_SteadyState)->Range(8, 1024);

static void BM_Doppler_SteadyState(benchmark::State& state) {
  AudioBenchEnv env;
  if (!env.valid()) {
    state.SkipWithError("SDL3_mixer dummy-driver init failed");
    return;
  }

  BenchScene scene;
  BuildSceneCommon(scene, env, static_cast<int>(state.range(0)), /*withRigidBody=*/true);
  DopplerSystem system;
  StubContext impl(&scene.registry);
  const ContextFacade ctx(&impl);

  scene.dopplerQuery->ForEach([&](const Entity e, const GlobalTransformComponent& tf, const RigidBodyComponent& rb,
                                  AudioSourceComponent& src,
                                  AudioSinkComponent& sink) { system(ctx, e, tf, rb, src, sink); });

  for (auto _ : state) {
    scene.dopplerQuery->ForEach([&](const Entity e, const GlobalTransformComponent& tf, const RigidBodyComponent& rb,
                                    AudioSourceComponent& src,
                                    AudioSinkComponent& sink) { system(ctx, e, tf, rb, src, sink); });
  }
  state.SetItemsProcessed(static_cast<int64_t>(state.iterations()) * state.range(0));
}

BENCHMARK(BM_Doppler_SteadyState)->Range(8, 1024);
