#pragma once

#include <cstdint>
#include <string>

// XOR decryption for Lua scripts protected at package time by OctarinePackage.cmake's
// OCTARINE_LUA_XOR_KEY step. Only active when the key is compiled in — shipped builds with
// OCTARINE_PROTECT_SCRIPTS=ON and a non-empty key. No-op in dev builds (the define is absent).
//
// Encryption and decryption are the same operation (XOR is symmetric). The key at position i is
// (base_key + i) % 256. Note: base_key is recoverable from the known Lua magic header — this is
// obfuscation against automated tooling, not cryptographic protection.
inline void DecryptLuaBytes(std::string& bytes) {
#if defined(OCTARINE_LUA_XOR_KEY)
  constexpr auto key = static_cast<uint8_t>(OCTARINE_LUA_XOR_KEY);
  for (std::size_t i = 0; i < bytes.size(); ++i) bytes[i] ^= static_cast<uint8_t>((key + i) & 0xFF);
#else
  (void)bytes;
#endif
}
