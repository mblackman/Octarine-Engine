// Smoke checks for the DevListenServer (Stage 6 PR-A). gtest-free; failed-check count is the
// exit code. Mirrors the style of ProcessTest / ProjectIniTest.
//
// Server is OCTARINE_SHIPPED-gated; this test target builds only when the engine target also
// builds it. Test spawns the server on an ephemeral port, dials in over loopback, and validates
// the hello + ping ops round-trip.

#include "Dev/DevListenServer.h"

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
using socket_t = SOCKET;
using io_size_t = int;
using io_ssize_t = int;
constexpr socket_t kInvalidSocket = INVALID_SOCKET;
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
using socket_t = int;
using io_size_t = std::size_t;
using io_ssize_t = ssize_t;
constexpr socket_t kInvalidSocket = -1;
#endif

#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>

namespace {
int g_failures = 0;

void Check(const bool cond, const std::string& what) {
  if (cond) {
    std::cout << "  ok   " << what << "\n";
  } else {
    std::cerr << "  FAIL " << what << "\n";
    ++g_failures;
  }
}

void CloseSock(socket_t s) {
#ifdef _WIN32
  ::closesocket(s);
#else
  ::close(s);
#endif
}

bool WriteAll(socket_t s, const void* in, std::size_t n) {
  const auto* p = static_cast<const char*>(in);
  while (n > 0) {
    const io_ssize_t sent = ::send(s, p, static_cast<io_size_t>(n), 0);
    if (sent <= 0) return false;
    p += sent;
    n -= static_cast<std::size_t>(sent);
  }
  return true;
}

bool ReadAll(socket_t s, void* out, std::size_t n) {
  auto* p = static_cast<char*>(out);
  while (n > 0) {
    const io_ssize_t got = ::recv(s, p, static_cast<io_size_t>(n), 0);
    if (got <= 0) return false;
    p += got;
    n -= static_cast<std::size_t>(got);
  }
  return true;
}

// Sends `op` + body, reads back `op` + body. Returns the reply body (excluding the 4-byte op
// header). Empty on failure (g_failures bumped via Check).
std::string RoundTrip(std::uint16_t port, std::uint32_t op, const std::string& body) {
  socket_t s = ::socket(AF_INET, SOCK_STREAM, 0);
  if (s == kInvalidSocket) {
    Check(false, "RoundTrip: socket()");
    return {};
  }
  sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

  if (::connect(s, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != 0) {
    Check(false, "RoundTrip: connect()");
    CloseSock(s);
    return {};
  }

  const std::uint32_t length = 4 + static_cast<std::uint32_t>(body.size());
  if (!WriteAll(s, &length, sizeof(length)) || !WriteAll(s, &op, sizeof(op)) ||
      (body.size() > 0 && !WriteAll(s, body.data(), body.size()))) {
    Check(false, "RoundTrip: WriteAll(request)");
    CloseSock(s);
    return {};
  }

  std::uint32_t reply_len = 0;
  if (!ReadAll(s, &reply_len, sizeof(reply_len))) {
    Check(false, "RoundTrip: ReadAll(reply_len)");
    CloseSock(s);
    return {};
  }
  std::uint32_t reply_op = 0;
  if (!ReadAll(s, &reply_op, sizeof(reply_op))) {
    Check(false, "RoundTrip: ReadAll(reply_op)");
    CloseSock(s);
    return {};
  }
  if (reply_op != op) {
    Check(false, "RoundTrip: reply op echoes request");
    CloseSock(s);
    return {};
  }
  const std::uint32_t reply_body_len = reply_len - 4;
  std::string out(reply_body_len, '\0');
  if (reply_body_len > 0 && !ReadAll(s, out.data(), reply_body_len)) {
    Check(false, "RoundTrip: ReadAll(reply_body)");
    CloseSock(s);
    return {};
  }
  CloseSock(s);
  return out;
}
}  // namespace

int main() {
#ifdef _WIN32
  WSADATA wsadata{};
  ::WSAStartup(MAKEWORD(2, 2), &wsadata);
#endif

  using namespace octarine::dev;
  DevListenServer server;
  ServerOptions opts;
  opts.port = 0;            // ephemeral
  opts.listen_all = false;  // loopback only

  Check(server.Start(opts), "Start() on ephemeral port succeeds");
  const std::uint16_t port = server.BoundPort();
  Check(port != 0, "BoundPort() resolves a real port");
  Check(server.IsRunning(), "IsRunning() true after Start");

  {
    const std::string banner = RoundTrip(port, static_cast<std::uint32_t>(OpCode::Hello), {});
    Check(banner == std::string(kHelloBanner), "Hello returns the engine banner");
  }

  {
    // 8-byte nonce echoed back unchanged.
    const std::string nonce = "OCTARINE";
    const std::string echo = RoundTrip(port, static_cast<std::uint32_t>(OpCode::Ping), nonce);
    Check(echo == nonce, "Ping echoes the client nonce");
  }

  server.Stop();
  Check(!server.IsRunning(), "IsRunning() false after Stop");

  // Restart on a fresh port to confirm Stop+Start cycle is clean.
  Check(server.Start(opts), "Start() after Stop succeeds again");
  server.Stop();

#ifdef _WIN32
  ::WSACleanup();
#endif

  return g_failures;
}
