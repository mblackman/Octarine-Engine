#pragma once

#include <cstdint>
#include <cstring>
#include <string>

// XOR decryption for Lua scripts protected at package time by OctarinePackage.cmake's
// OCTARINE_LUA_XOR_KEY step. Only active when the key is compiled in — shipped builds with
// OCTARINE_PROTECT_SCRIPTS=ON and a non-empty key. No-op in dev builds (the define is absent).
//
// Encrypted payloads are prefixed at package time with a 4-byte magic sentinel (0x1B 'O' 'C' 'T').
// DecryptLuaBytes() only transforms files carrying that sentinel: it strips the prefix and reverses
// the rolling XOR (key at position i is (base_key + i) % 256). Files WITHOUT the sentinel — plain
// Lua source, stock \x1bLua bytecode, or any future un-encrypted payload such as user mods — pass
// through untouched, so decryption stays robust even when only part of the script surface is
// encrypted. base_key is recoverable from the known Lua header, so this is obfuscation against
// automated tooling, not cryptographic protection.
inline void DecryptLuaBytes(std::string& bytes) {
#if defined(OCTARINE_LUA_XOR_KEY)
  static constexpr unsigned char kMagic[] = {0x1B, 'O', 'C', 'T'};
  constexpr std::size_t kMagicLen = sizeof(kMagic);
  if (bytes.size() < kMagicLen || std::memcmp(bytes.data(), kMagic, kMagicLen) != 0) {
    return;  // not an Octarine-encrypted payload (plain source / stock bytecode / mods) — leave as-is
  }
  bytes.erase(0, kMagicLen);
  constexpr auto key = static_cast<uint8_t>(OCTARINE_LUA_XOR_KEY);
  for (std::size_t i = 0; i < bytes.size(); ++i) bytes[i] ^= static_cast<uint8_t>((key + i) & 0xFF);
#else
  (void)bytes;
#endif
}
