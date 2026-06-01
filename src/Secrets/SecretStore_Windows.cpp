#include "Secrets/SecretStore.h"

#ifdef _WIN32

#include <SDL3/SDL_filesystem.h>

#include "General/Logger.h"

// dpapi.h pulls wincrypt symbols (CryptProtectData / CryptUnprotectData). crypt32 link added in
// the top-level CMakeLists alongside the Windows-only SRC_FILES append.
// windows.h MUST precede dpapi.h (dpapi.h depends on its types); keep this order.
// clang-format off
#include <windows.h>
#include <dpapi.h>
// clang-format on

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <string>
#include <system_error>
#include <vector>

namespace octarine::secrets {
namespace {
std::filesystem::path SecretsDir() {
  char* prefPath = SDL_GetPrefPath("Octarine", "Engine");
  if (!prefPath) {
    return {};
  }
  std::filesystem::path p = std::filesystem::path(prefPath) / "secrets";
  SDL_free(prefPath);

  std::error_code ec;
  std::filesystem::create_directories(p, ec);
  return p;
}

// Filesystem-safe key filename: replace anything outside [A-Za-z0-9._-] with '_'. Keeps
// round-tripping deterministic without needing a separate index file.
std::string SanitizeKey(std::string_view key) {
  std::string out;
  out.reserve(key.size());
  for (char c : key) {
    const unsigned char u = static_cast<unsigned char>(c);
    const bool ok =
        (u >= 'A' && u <= 'Z') || (u >= 'a' && u <= 'z') || (u >= '0' && u <= '9') || u == '.' || u == '-' || u == '_';
    out.push_back(ok ? c : '_');
  }
  return out;
}

std::filesystem::path PathFor(std::string_view key) {
  auto dir = SecretsDir();
  if (dir.empty()) {
    return {};
  }
  return dir / (SanitizeKey(key) + ".dat");
}
}  // namespace

bool IsAvailable() { return true; }

bool Set(std::string_view key, std::string_view value) {
  const auto path = PathFor(key);
  if (path.empty()) {
    return false;
  }

  DATA_BLOB in{};
  in.pbData = const_cast<BYTE*>(reinterpret_cast<const BYTE*>(value.data()));
  in.cbData = static_cast<DWORD>(value.size());

  DATA_BLOB out{};
  if (!CryptProtectData(&in, L"OctarineEngine", nullptr, nullptr, nullptr, CRYPTPROTECT_UI_FORBIDDEN, &out)) {
    Logger::Error("SecretStore: CryptProtectData failed (err=" + std::to_string(GetLastError()) + ")");
    return false;
  }

  std::ofstream f(path, std::ios::binary | std::ios::trunc);
  if (!f.is_open()) {
    LocalFree(out.pbData);
    Logger::Error("SecretStore: cannot open " + path.string());
    return false;
  }
  f.write(reinterpret_cast<const char*>(out.pbData), static_cast<std::streamsize>(out.cbData));
  const bool ok = f.good();
  LocalFree(out.pbData);
  return ok;
}

std::optional<std::string> Get(std::string_view key) {
  const auto path = PathFor(key);
  if (path.empty()) {
    return std::nullopt;
  }
  std::error_code ec;
  if (!std::filesystem::exists(path, ec)) {
    return std::nullopt;
  }

  std::ifstream f(path, std::ios::binary);
  if (!f.is_open()) {
    return std::nullopt;
  }
  std::vector<char> ciphertext((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());

  DATA_BLOB in{};
  in.pbData = reinterpret_cast<BYTE*>(ciphertext.data());
  in.cbData = static_cast<DWORD>(ciphertext.size());

  DATA_BLOB out{};
  if (!CryptUnprotectData(&in, nullptr, nullptr, nullptr, nullptr, CRYPTPROTECT_UI_FORBIDDEN, &out)) {
    Logger::Error("SecretStore: CryptUnprotectData failed (err=" + std::to_string(GetLastError()) + ")");
    return std::nullopt;
  }
  std::string result(reinterpret_cast<const char*>(out.pbData), out.cbData);
  LocalFree(out.pbData);
  return result;
}

bool Clear(std::string_view key) {
  const auto path = PathFor(key);
  if (path.empty()) {
    return false;
  }
  std::error_code ec;
  std::filesystem::remove(path, ec);
  // remove() returning false because the file did not exist is still success per our contract.
  return !ec || ec == std::errc::no_such_file_or_directory;
}
}  // namespace octarine::secrets

#endif  // _WIN32
