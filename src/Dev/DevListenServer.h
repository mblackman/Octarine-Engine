#pragma once

#ifndef OCTARINE_SHIPPED

// DevListenServer — Stage 6 of ai/EditorBuildAndDeployPlan.md (PR-A foundation: TCP listener +
// hello/ping handshake; PR-B will add eval_lua / reload_scene / push_script / push_asset).
//
// Compile-time stripped from shipped builds (#ifndef OCTARINE_SHIPPED). When enabled, owns a
// background listener thread bound to 127.0.0.1:<port> (or 0.0.0.0:<port> with --dev-listen-all)
// and serves a tiny length-prefixed binary protocol:
//
//     each message:  uint32 LE length | uint32 LE op | body[length-4]
//     hello reply:   op=Hello, body = "OCTARINE_DEV_LISTEN/v1\n"
//     ping reply:    op=Pong,  body = client nonce (8 bytes echoed back)
//
// I/O lives on the listener thread; PR-A ops are pure I/O so they reply directly. Pump() exists
// on the main thread as the seam PR-B will route commands-that-touch-engine-state through (Lua
// eval, scene reload, asset/script writes) once those land. No engine state is touched off the
// main thread today.

#include <cstdint>
#include <memory>

namespace octarine::dev {
enum class OpCode : std::uint32_t {
  Hello = 1,  // body: empty       -> reply Hello with banner
  Ping = 2,   // body: u64 LE nonce -> reply Pong with same nonce
              // Reserved by PR-B (do not reuse): 10 EvalLua, 11 ReloadScene, 12 PushScript, 13 PushAsset.
};

struct ServerOptions {
  std::uint16_t port = 0;   // 0 = pick ephemeral (BoundPort() reads back after Start)
  bool listen_all = false;  // false = bind 127.0.0.1; true = 0.0.0.0 (warned at Start)
};

class DevListenServer {
 public:
  DevListenServer();
  ~DevListenServer();

  DevListenServer(const DevListenServer&) = delete;
  DevListenServer& operator=(const DevListenServer&) = delete;
  DevListenServer(DevListenServer&&) noexcept;
  DevListenServer& operator=(DevListenServer&&) noexcept;

  // Spawns the listener thread bound per `opts`. Returns true on bind+listen success and
  // populates BoundPort(). Returns false (and logs) on socket / bind / listen failure.
  // Idempotent no-op (returning true) when already running.
  bool Start(const ServerOptions& opts);

  // Closes the listening socket, signals the thread to exit, and joins. Safe to call from
  // any thread; idempotent.
  void Stop();

  bool IsRunning() const;
  std::uint16_t BoundPort() const;

  // Per-frame drain for commands that must run on the main thread. PR-A queues nothing
  // (hello/ping reply on the listener thread); PR-B wires eval_lua / reload_scene through
  // a SPSC ring drained here.
  void Pump();

 private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};

// Engine banner returned by the Hello op. Versioned so PR-B can bump and clients can gate.
inline constexpr const char* kHelloBanner = "OCTARINE_DEV_LISTEN/v1\n";
}  // namespace octarine::dev

#endif  // !OCTARINE_SHIPPED
