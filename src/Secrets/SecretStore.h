#pragma once

// SecretStore — Stage 5 PR-C of ai/EditorBuildAndDeployPlan.md.
//
// Per-platform credential storage for editor-side build tooling. v1 holds the Android signing
// keystore credentials (path + passwords + alias) so a dev can ship a release AAB from the
// Export Build modal without exporting OCTARINE_ANDROID_* in their shell.
//
// Backends:
//   Windows — DPAPI (CryptProtectData) writing per-key .dat files under SDL_GetPrefPath/secrets/.
//   macOS   — Keychain Services (SecKeychainAddGenericPassword) under service "OctarineEngine".
//   Linux   — Not available. Set/Get/Clear return false; IsAvailable() returns false. Editor UI
//             falls back to surfacing the env-var contract instead of offering a save UI.
//
// API stays narrow on purpose: arbitrary string keys -> arbitrary string values. The caller picks
// the namespacing convention (current users prefix with "octarine.android." to avoid collisions
// with whatever else lands in the store later).

#include <optional>
#include <string>
#include <string_view>

namespace octarine::secrets {
// True when the host platform has a real secret-storage backend wired up. False on Linux v1
// (and on Windows / macOS when the backend reports a runtime error during a probe).
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
