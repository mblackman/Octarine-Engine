#pragma once

// Per-platform credential storage for editor-side build tooling (e.g. Android signing
// keystore credentials).
//
// Backends:
//   Windows — DPAPI (CryptProtectData) writing per-key .dat files under SDL_GetPrefPath/secrets/.
//   macOS   — Keychain Services (SecKeychainAddGenericPassword) under service "OctarineEngine".
//   Linux   — Unsupported; Set/Get/Clear return false and IsAvailable() returns false.
//
// Keys and values are arbitrary strings. Callers typically namespace keys (e.g. "octarine.android.").

#include <optional>
#include <string>
#include <string_view>

namespace octarine::secrets {
// True when the host platform has an active credential storage backend available.
bool IsAvailable();

// Stores `value` keyed by `key`, overwriting any existing entry. Returns true on success.
// Empty `value` is allowed and stores the empty string (distinct from "no entry"); callers
// who want to remove should use Clear().
bool Set(std::string_view key, std::string_view value);

// Returns the stored value for `key`, or nullopt when no entry exists or decryption failed.
std::optional<std::string> Get(std::string_view key);

// Removes the stored entry for `key`. Returns true on success or when the entry did not exist.
bool Clear(std::string_view key);
}  // namespace octarine::secrets
