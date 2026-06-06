#include "Dev/DevListenClient.h"

#ifndef OCTARINE_SHIPPED

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
using client_sock_t = SOCKET;
constexpr client_sock_t kClientInvalidSock = INVALID_SOCKET;
inline void ClientCloseSocket(client_sock_t s) { ::closesocket(s); }
#else
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
using client_sock_t = int;
constexpr client_sock_t kClientInvalidSock = -1;
inline void ClientCloseSocket(client_sock_t s) { ::close(s); }
#endif

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

#include "Dev/DevListenServer.h"

namespace octarine::dev {
namespace {

#ifdef _WIN32
void WsaInit() {
  WSADATA d{};
  ::WSAStartup(MAKEWORD(2, 2), &d);
}
void WsaCleanup() { ::WSACleanup(); }
#else
void WsaInit() {}
void WsaCleanup() {}
#endif

// Connect to host:port with a 1-second timeout. Returns kClientInvalidSock on failure.
client_sock_t Connect(const std::string& host_port) {
  const auto colon = host_port.rfind(':');
  if (colon == std::string::npos || colon + 1 >= host_port.size()) return kClientInvalidSock;
  const std::string host = host_port.substr(0, colon);
  const std::string port_str = host_port.substr(colon + 1);

  addrinfo hints{};
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  addrinfo* res = nullptr;
  if (::getaddrinfo(host.c_str(), port_str.c_str(), &hints, &res) != 0 || res == nullptr) {
    return kClientInvalidSock;
  }

  client_sock_t s = ::socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if (s == kClientInvalidSock) {
    ::freeaddrinfo(res);
    return kClientInvalidSock;
  }

#ifdef _WIN32
  DWORD ms = 1000;
  ::setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<const char*>(&ms), sizeof(ms));
  ::setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&ms), sizeof(ms));
#else
  struct timeval tv {};
  tv.tv_sec = 1;
  ::setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
  ::setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
#endif

#ifdef _WIN32
  if (::connect(s, res->ai_addr, static_cast<int>(res->ai_addrlen)) != 0) {
#else
  if (::connect(s, res->ai_addr, res->ai_addrlen) != 0) {
#endif
    ::freeaddrinfo(res);
    ClientCloseSocket(s);
    return kClientInvalidSock;
  }
  ::freeaddrinfo(res);
  return s;
}

bool WriteAll(client_sock_t s, const void* buf, std::size_t n) {
  const auto* p = static_cast<const char*>(buf);
  while (n > 0) {
#ifdef _WIN32
    const int sent = ::send(s, p, static_cast<int>(n), 0);
#else
    const ssize_t sent = ::send(s, p, n, 0);
#endif
    if (sent <= 0) return false;
    p += sent;
    n -= static_cast<std::size_t>(sent);
  }
  return true;
}

bool ReadAll(client_sock_t s, void* buf, std::size_t n) {
  auto* p = static_cast<char*>(buf);
  while (n > 0) {
#ifdef _WIN32
    const int got = ::recv(s, p, static_cast<int>(n), 0);
#else
    const ssize_t got = ::recv(s, p, n, 0);
#endif
    if (got <= 0) return false;
    p += got;
    n -= static_cast<std::size_t>(got);
  }
  return true;
}

bool WriteFrame(client_sock_t s, std::uint32_t op, const void* body, std::uint32_t body_len) {
  const std::uint32_t length = 4 + body_len;
  if (!WriteAll(s, &length, sizeof(length))) return false;
  if (!WriteAll(s, &op, sizeof(op))) return false;
  if (body_len > 0 && !WriteAll(s, body, body_len)) return false;
  return true;
}

// Read one reply frame. Returns false on socket error; fills reply_op and reply_body on success.
bool ReadFrame(client_sock_t s, std::uint32_t& reply_op, std::vector<char>& reply_body) {
  std::uint32_t length = 0;
  if (!ReadAll(s, &length, sizeof(length))) return false;
  if (length < sizeof(std::uint32_t)) return false;
  if (!ReadAll(s, &reply_op, sizeof(reply_op))) return false;
  const std::uint32_t body_len = length - static_cast<std::uint32_t>(sizeof(std::uint32_t));
  reply_body.resize(body_len);
  if (body_len > 0 && !ReadAll(s, reply_body.data(), body_len)) return false;
  return true;
}

// Send a frame and read the reply; returns an error result on socket failure or error op.
ClientResult SendAndRecv(const std::string& host_port, std::uint32_t op, const void* body, std::uint32_t body_len) {
  WsaInit();
  client_sock_t s = Connect(host_port);
  if (s == kClientInvalidSock) {
    WsaCleanup();
    return {false, "connect failed"};
  }

  if (!WriteFrame(s, op, body, body_len)) {
    ClientCloseSocket(s);
    WsaCleanup();
    return {false, "send failed"};
  }

  std::uint32_t reply_op = 0;
  std::vector<char> reply_body;
  if (!ReadFrame(s, reply_op, reply_body)) {
    ClientCloseSocket(s);
    WsaCleanup();
    return {false, "recv failed"};
  }
  ClientCloseSocket(s);
  WsaCleanup();

  if (reply_op == kErrorReplyOp) {
    const std::string msg(reply_body.begin(), reply_body.end());
    return {false, msg.empty() ? "server error" : msg};
  }

  // For push/reload ops the reply body is: byte[0]=ok, rest=utf8 message
  if (!reply_body.empty()) {
    const bool ok = (static_cast<unsigned char>(reply_body[0]) != 0);
    const std::string msg(reply_body.begin() + 1, reply_body.end());
    return {ok, msg};
  }
  return {true, {}};
}

ClientResult PushOp(const std::string& host_port, std::uint32_t op, const std::string& relative_path,
                    const std::vector<char>& content) {
  // Body: uint32_le path_len | path[path_len] | file_bytes[rest]
  const std::uint32_t path_len = static_cast<std::uint32_t>(relative_path.size());
  const std::uint32_t total =
      static_cast<std::uint32_t>(sizeof(path_len)) + path_len + static_cast<std::uint32_t>(content.size());

  std::vector<char> body(total);
  std::memcpy(body.data(), &path_len, sizeof(path_len));
  std::memcpy(body.data() + sizeof(path_len), relative_path.data(), path_len);
  if (!content.empty()) {
    std::memcpy(body.data() + sizeof(path_len) + path_len, content.data(), content.size());
  }

  return SendAndRecv(host_port, op, body.data(), total);
}

}  // namespace

ClientResult DevListenClient::Ping(const std::string& host_port) {
  WsaInit();
  client_sock_t s = Connect(host_port);
  if (s == kClientInvalidSock) {
    WsaCleanup();
    return {false, "connect failed"};
  }

  // Ping body: uint64_le nonce
  const std::uint64_t nonce = 0xDEADBEEFCAFEBABEull;
  if (!WriteFrame(s, static_cast<std::uint32_t>(OpCode::Ping), &nonce, sizeof(nonce))) {
    ClientCloseSocket(s);
    WsaCleanup();
    return {false, "send failed"};
  }

  std::uint32_t reply_op = 0;
  std::vector<char> reply_body;
  if (!ReadFrame(s, reply_op, reply_body)) {
    ClientCloseSocket(s);
    WsaCleanup();
    return {false, "recv failed"};
  }
  ClientCloseSocket(s);
  WsaCleanup();

  if (reply_op == kErrorReplyOp) return {false, "server error"};
  // Verify nonce echo
  if (reply_body.size() == sizeof(nonce)) {
    std::uint64_t echoed = 0;
    std::memcpy(&echoed, reply_body.data(), sizeof(echoed));
    if (echoed == nonce) return {true, {}};
  }
  return {true, {}};  // connected even if nonce check inconclusive
}

ClientResult DevListenClient::PushScript(const std::string& host_port, const std::string& relative_path,
                                         const std::vector<char>& content) {
  return PushOp(host_port, static_cast<std::uint32_t>(OpCode::PushScript), relative_path, content);
}

ClientResult DevListenClient::PushAsset(const std::string& host_port, const std::string& relative_path,
                                        const std::vector<char>& content) {
  return PushOp(host_port, static_cast<std::uint32_t>(OpCode::PushAsset), relative_path, content);
}

ClientResult DevListenClient::ReloadScene(const std::string& host_port) {
  return SendAndRecv(host_port, static_cast<std::uint32_t>(OpCode::ReloadScene), nullptr, 0);
}

ClientResult DevListenClient::EvalLua(const std::string& host_port, const std::string& src) {
  return SendAndRecv(host_port, static_cast<std::uint32_t>(OpCode::EvalLua), src.data(),
                     static_cast<std::uint32_t>(src.size()));
}

}  // namespace octarine::dev

#endif  // !OCTARINE_SHIPPED
