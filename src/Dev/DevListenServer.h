#pragma once

#ifndef OCTARINE_SHIPPED

// DevListenServer — editor dev-iteration TCP listener (compile-time stripped from shipped builds
// via #ifndef OCTARINE_SHIPPED). When enabled, owns a background listener thread bound to
// 127.0.0.1:<port> and serves a tiny length-prefixed
// binary protocol:
//
//     each message:  uint32 LE length | uint32 LE op | body[length-4]
//     hello reply:   op=Hello, body = "OCTARINE_DEV_LISTEN/v1\n"
//     ping reply:    op=Pong,  body = client nonce echoed back
//
// Pure-I/O ops (Hello/Ping) reply directly on the listener thread. The engine-mutating ops
// (EvalLua / ReloadScene / PushScript / PushAsset) cannot touch the Registry / sol::state /
// AssetManager off the main thread, so the listener parks each one on a queue and blocks until
// the main thread drains it in Pump(); Pump runs the command and hands the reply back so the
// listener can write it and close. See `kPush*` body layout on each opcode below.

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace octarine::dev {
enum class OpCode : std::uint32_t {
  Hello = 1,  // body: empty        -> reply Hello with banner
  Ping = 2,   // body: u64 LE nonce -> reply Pong with same nonce
  // Engine-mutating ops, parked by the listener and dispatched to the main thread via Pump():
  EvalLua = 10,      // body: lua source        -> reply { u8 ok, utf8 result/error }
  ReloadScene = 11,  // body: empty             -> re-runs the active scene; reply { u8 ok }
  PushScript = 12,   // body: u32 path_len | path | bytes -> writes script + forces reload; reply { u8 ok }
  PushAsset = 13,    // body: u32 path_len | path | bytes -> writes asset + reloads it; reply { u8 ok }
};

// Handler the engine installs to apply an engine-mutating op on the main thread. Invoked from
// Pump() for each parked command; receives the op + raw body and fills reply_op/reply_body (the
// frame written back to the client). Keeping this a std::function is what lets the transport layer
// carry zero engine dependencies — the Registry/sol/AssetManager-touching logic lives in
// DevListenDispatch (engine layer), installed via SetCommandHandler.
using CommandHandler =
    std::function<void(OpCode op, const std::vector<char>& body, std::uint32_t& reply_op, std::string& reply_body)>;

struct ServerOptions {
  std::uint16_t port = 0;  // 0 = pick ephemeral (BoundPort() reads back after Start)
                           // Always binds 127.0.0.1 — no LAN-exposure option.
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

  // Install the main-thread handler for engine-mutating ops. Call before Start() (or any time;
  // commands park until a handler exists). Without one, parked ops reply with an error frame.
  void SetCommandHandler(CommandHandler handler);

  // Per-frame drain for engine-mutating commands the listener parked. Runs each queued op through
  // the installed handler on the calling (main) thread, then unblocks the listener with its reply.
  // Cheap when the queue is empty; call every frame. Hello/Ping never reach here.
  void Pump();

 private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};

// Engine banner returned by the Hello op. Versioned so clients can gate on protocol capabilities.
inline constexpr const char* kHelloBanner = "OCTARINE_DEV_LISTEN/v1\n";

// Reply op written back for an unknown opcode or a command with no handler installed. Distinct
// from every real OpCode so clients can detect "server refused" without parsing the body.
inline constexpr std::uint32_t kErrorReplyOp = 0xFFFFFFFFu;
}  // namespace octarine::dev

#endif  // !OCTARINE_SHIPPED
