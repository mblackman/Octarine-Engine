#include "Dev/DevListenServer.h"

#ifndef OCTARINE_SHIPPED

#include "General/Logger.h"

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
using socket_t = SOCKET;
using io_size_t = int;   // recv/send `len` arg type on Winsock
using io_ssize_t = int;  // recv/send return type on Winsock
constexpr socket_t kInvalidSocket = INVALID_SOCKET;
constexpr int kSocketError = SOCKET_ERROR;
#else
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
using socket_t = int;
using io_size_t = std::size_t;  // recv/send `len` arg type on POSIX
using io_ssize_t = ssize_t;     // recv/send return type on POSIX
constexpr socket_t kInvalidSocket = -1;
constexpr int kSocketError = -1;
#endif

#include <atomic>
#include <cstdint>
#include <cstring>
#include <string>
#include <thread>
#include <utility>
#include <vector>

namespace octarine::dev {
namespace {
void CloseSocket(socket_t s) {
  if (s == kInvalidSocket) return;
#ifdef _WIN32
  ::closesocket(s);
#else
  ::close(s);
#endif
}

// Toggle blocking mode. The listen socket is set non-blocking so accept() never parks the
// listener thread (Stop() can't wake a blocked accept() by closing the fd on Linux — close()
// does not interrupt accept() there, unlike Winsock). Accepted client sockets are forced
// back to blocking so the ReadAll/WriteAll loops keep their simple >0 / <=0 semantics
// (a non-blocking client would spuriously fail ReadAll with EWOULDBLOCK before data lands;
// Windows-accepted sockets inherit the listener's non-blocking flag, so this is required).
void SetSocketBlocking(socket_t s, bool blocking) {
#ifdef _WIN32
  u_long mode = blocking ? 0u : 1u;
  ::ioctlsocket(s, FIONBIO, &mode);
#else
  const int flags = ::fcntl(s, F_GETFL, 0);
  if (flags < 0) return;
  ::fcntl(s, F_SETFL, blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK));
#endif
}

int LastSocketError() {
#ifdef _WIN32
  return ::WSAGetLastError();
#else
  return errno;
#endif
}

// Reads exactly `n` bytes from `s` into `out`. Returns false if the peer closed early or
// the socket errored. Tiny loop is fine — frames are small in PR-A (<= 12 bytes).
bool ReadAll(socket_t s, void* out, std::size_t n) {
  auto* p = static_cast<char*>(out);
  while (n > 0) {
    const io_ssize_t got = ::recv(s, p, static_cast<io_size_t>(n), 0);
    if (got <= 0) {
      return false;
    }
    p += got;
    n -= static_cast<std::size_t>(got);
  }
  return true;
}

bool WriteAll(socket_t s, const void* in, std::size_t n) {
  const auto* p = static_cast<const char*>(in);
  while (n > 0) {
    const io_ssize_t sent = ::send(s, p, static_cast<io_size_t>(n), 0);
    if (sent <= 0) {
      return false;
    }
    p += sent;
    n -= static_cast<std::size_t>(sent);
  }
  return true;
}

bool WriteFrame(socket_t s, std::uint32_t op, const void* body, std::uint32_t body_len) {
  const std::uint32_t length = 4 + body_len;  // op (4) + body
  if (!WriteAll(s, &length, sizeof(length))) return false;
  if (!WriteAll(s, &op, sizeof(op))) return false;
  if (body_len > 0 && !WriteAll(s, body, body_len)) return false;
  return true;
}

#ifdef _WIN32
// Reference-counted WSAStartup so multiple servers (or test threads) can share one init.
std::atomic<int> g_wsa_refs{0};

void WsaAcquire() {
  if (g_wsa_refs.fetch_add(1, std::memory_order_acq_rel) == 0) {
    WSADATA d{};
    ::WSAStartup(MAKEWORD(2, 2), &d);
  }
}
void WsaRelease() {
  if (g_wsa_refs.fetch_sub(1, std::memory_order_acq_rel) == 1) {
    ::WSACleanup();
  }
}
#else
void WsaAcquire() {}
void WsaRelease() {}
#endif
}  // namespace

struct DevListenServer::Impl {
  std::atomic<bool> running{false};
  std::atomic<bool> stop_requested{false};
  std::uint16_t bound_port{0};
  socket_t listen_sock{kInvalidSocket};
  std::thread listener;
};

DevListenServer::DevListenServer() : impl_(std::make_unique<Impl>()) {}

DevListenServer::~DevListenServer() { Stop(); }

DevListenServer::DevListenServer(DevListenServer&&) noexcept = default;
DevListenServer& DevListenServer::operator=(DevListenServer&&) noexcept = default;

bool DevListenServer::IsRunning() const { return impl_ && impl_->running.load(std::memory_order_acquire); }

std::uint16_t DevListenServer::BoundPort() const { return impl_ ? impl_->bound_port : 0; }

void DevListenServer::Pump() {
  // PR-A reserved hook: no off-thread commands queued yet. PR-B will drain a SPSC ring of
  // EvalLua / ReloadScene / PushScript / PushAsset requests here so they execute on the main
  // thread (Registry / sol::state / AssetManager are not thread-safe).
}

bool DevListenServer::Start(const ServerOptions& opts) {
  if (!impl_) return false;
  if (impl_->running.load(std::memory_order_acquire)) return true;

  WsaAcquire();

  socket_t s = ::socket(AF_INET, SOCK_STREAM, 0);
  if (s == kInvalidSocket) {
    Logger::Error("DevListenServer: socket() failed (err=" + std::to_string(LastSocketError()) + ")");
    WsaRelease();
    return false;
  }

  int reuse = 1;
  ::setsockopt(s, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&reuse), sizeof(reuse));

  sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(opts.port);
  addr.sin_addr.s_addr = opts.listen_all ? htonl(INADDR_ANY) : htonl(INADDR_LOOPBACK);

  if (::bind(s, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == kSocketError) {
    Logger::Error("DevListenServer: bind() failed (err=" + std::to_string(LastSocketError()) +
                  ", port=" + std::to_string(opts.port) + ")");
    CloseSocket(s);
    WsaRelease();
    return false;
  }
  if (::listen(s, 4) == kSocketError) {
    Logger::Error("DevListenServer: listen() failed (err=" + std::to_string(LastSocketError()) + ")");
    CloseSocket(s);
    WsaRelease();
    return false;
  }

  // Non-blocking listen socket: the listener thread waits in select() and only accept()s when
  // a connection is pending, so it never parks. Stop() then just flips stop_requested and the
  // loop exits within one select() timeout — no cross-thread fd close to race the accept().
  SetSocketBlocking(s, false);

  // Read back the ephemeral port the kernel picked when opts.port == 0 so callers/tests can
  // dial in without racing the bind.
  sockaddr_in bound{};
  socklen_t blen = sizeof(bound);
  if (::getsockname(s, reinterpret_cast<sockaddr*>(&bound), &blen) == 0) {
    impl_->bound_port = ntohs(bound.sin_port);
  }

  if (opts.listen_all) {
    Logger::Warn("DevListenServer: bound 0.0.0.0:" + std::to_string(impl_->bound_port) +
                 " — exposed to LAN. Prefer 127.0.0.1 unless cross-host dev is intentional.");
  } else {
    Logger::Info("DevListenServer: bound 127.0.0.1:" + std::to_string(impl_->bound_port));
  }

  impl_->listen_sock = s;
  impl_->stop_requested.store(false, std::memory_order_release);
  impl_->running.store(true, std::memory_order_release);

  impl_->listener = std::thread([this]() {
    socket_t ls = impl_->listen_sock;
    while (!impl_->stop_requested.load(std::memory_order_acquire)) {
      // Wait up to 100ms for a pending connection, then re-check stop_requested. This is
      // what makes Stop() prompt and deadlock-free: no blocking accept() to interrupt.
      fd_set readable;
      FD_ZERO(&readable);
      FD_SET(ls, &readable);
      timeval timeout{};
      timeout.tv_sec = 0;
      timeout.tv_usec = 100 * 1000;  // 100ms
      const int ready = ::select(static_cast<int>(ls + 1), &readable, nullptr, nullptr, &timeout);
      if (ready <= 0) {
        // 0 = timeout, <0 = interrupted/error; loop and re-test stop_requested.
        continue;
      }

      sockaddr_in client{};
      socklen_t clen = sizeof(client);
      socket_t cs = ::accept(ls, reinterpret_cast<sockaddr*>(&client), &clen);
      if (cs == kInvalidSocket) {
        // Connection withdrawn between select() and accept(), or a transient error;
        // not fatal — go back to waiting.
        continue;
      }
      // The accepted socket inherits the listener's non-blocking flag on Windows; force it
      // blocking so the ReadAll/WriteAll loops below keep their simple semantics.
      SetSocketBlocking(cs, true);
      // Handle one frame per connection for v1 simplicity; the matrix of
      // op x persistent-connection-state is small enough that closing+reopening is fine.
      std::uint32_t length = 0;
      if (!ReadAll(cs, &length, sizeof(length))) {
        CloseSocket(cs);
        continue;
      }
      if (length < 4 || length > 1024 * 1024) {
        Logger::Warn("DevListenServer: rejecting frame length=" + std::to_string(length));
        CloseSocket(cs);
        continue;
      }
      std::uint32_t op = 0;
      if (!ReadAll(cs, &op, sizeof(op))) {
        CloseSocket(cs);
        continue;
      }

      const std::uint32_t body_len = length - 4;
      std::vector<char> body(body_len);
      if (body_len > 0 && !ReadAll(cs, body.data(), body_len)) {
        CloseSocket(cs);
        continue;
      }

      switch (static_cast<OpCode>(op)) {
        case OpCode::Hello: {
          WriteFrame(cs, static_cast<std::uint32_t>(OpCode::Hello), kHelloBanner,
                     static_cast<std::uint32_t>(std::strlen(kHelloBanner)));
          break;
        }
        case OpCode::Ping: {
          // Echo the 8-byte client nonce back. Tolerate other body sizes (some clients
          // may ping with an empty body); just echo whatever they sent.
          WriteFrame(cs, static_cast<std::uint32_t>(OpCode::Ping), body.data(), body_len);
          break;
        }
        default: {
          Logger::Warn("DevListenServer: unknown op=" + std::to_string(op));
          const std::uint32_t err_op = 0xFFFFFFFFu;
          WriteFrame(cs, err_op, nullptr, 0);
          break;
        }
      }

      CloseSocket(cs);
    }
  });

  return true;
}

void DevListenServer::Stop() {
  if (!impl_) return;
  if (!impl_->running.exchange(false, std::memory_order_acq_rel)) {
    return;
  }
  impl_->stop_requested.store(true, std::memory_order_release);

  // Join first: the listener's select() loop notices stop_requested within one timeout tick
  // and returns on its own. Closing the listen socket only after the thread is gone avoids
  // the cross-thread close-during-accept race that hung the listener on Linux.
  if (impl_->listener.joinable()) {
    impl_->listener.join();
  }

  CloseSocket(impl_->listen_sock);
  impl_->listen_sock = kInvalidSocket;
  impl_->bound_port = 0;

  WsaRelease();
}
}  // namespace octarine::dev

#endif  // !OCTARINE_SHIPPED
