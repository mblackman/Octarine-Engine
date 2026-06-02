#include "Editor/Panels/EditorSigningPanel.h"

#ifdef OCTARINE_WITH_EDITOR

#include <cstring>

#include "Secrets/SecretStore.h"
#include "imgui.h"

namespace octarine::editor::panels {

void DrawSigningSettingsWindow(bool* p_open) {
  ImGui::SetNextWindowSize(ImVec2(540, 360), ImGuiCond_FirstUseEver);
  if (!ImGui::Begin("Signing Settings", p_open, ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::End();
    return;
  }

  // Per-field buffers persisted across frames. Populated with the placeholder string when an
  // entry already exists in SecretStore so the dev sees "value stored" rather than the secret
  // round-tripped into a visible password field.
  static char keystorePathBuf[260] = "";
  static char storePasswordBuf[128] = "";
  static char keyAliasBuf[64] = "";
  static char keyPasswordBuf[128] = "";
  static bool initialized = false;
  static bool keystorePathLoaded = false;

  constexpr const char* kKeyPath = "octarine.android.keystore_path";
  constexpr const char* kKeyStore = "octarine.android.store_password";
  constexpr const char* kKeyAlias = "octarine.android.key_alias";
  constexpr const char* kKeyPwd = "octarine.android.key_password";

  if (!octarine::secrets::IsAvailable()) {
    ImGui::TextColored(ImVec4(1.0F, 0.85F, 0.4F, 1.0F), "Secret storage is not available on this platform.");
    ImGui::TextWrapped(
        "Set the OCTARINE_ANDROID_* env vars (KEYSTORE_PATH, STORE_PASSWORD, KEY_ALIAS, "
        "KEY_PASSWORD) in the shell that launches the editor. The Export Build modal will "
        "pick them up via the editor's inherited environment.");
    ImGui::End();
    return;
  }

  // Lazy first-time load: the keystore path is non-secret-by-itself so we surface it directly;
  // password / alias fields stay blank with a "Stored" indicator beside them so the secrets
  // never round-trip through ImGui's input cache.
  if (!initialized) {
    if (auto v = octarine::secrets::Get(kKeyPath)) {
      std::snprintf(keystorePathBuf, sizeof(keystorePathBuf), "%s", v->c_str());
      keystorePathLoaded = true;
    }
    if (auto v = octarine::secrets::Get(kKeyAlias)) {
      std::snprintf(keyAliasBuf, sizeof(keyAliasBuf), "%s", v->c_str());
    }
    initialized = true;
  }

  const auto storedHint = [](const char* key) {
    return octarine::secrets::Get(key).has_value() ? "(stored)" : "(empty)";
  };

  ImGui::TextWrapped(
      "Android-release signing credentials. Stored via the OS secret backend (DPAPI on Windows, "
      "Keychain on macOS) and injected into the build subprocess as OCTARINE_ANDROID_* env vars.");
  ImGui::Separator();

  ImGui::SetNextItemWidth(360);
  ImGui::InputText("Keystore path", keystorePathBuf, sizeof(keystorePathBuf));

  ImGui::SetNextItemWidth(220);
  ImGui::InputText("Store password", storePasswordBuf, sizeof(storePasswordBuf), ImGuiInputTextFlags_Password);
  ImGui::SameLine();
  ImGui::TextDisabled("%s", storedHint(kKeyStore));

  ImGui::SetNextItemWidth(220);
  ImGui::InputText("Key alias", keyAliasBuf, sizeof(keyAliasBuf));

  ImGui::SetNextItemWidth(220);
  ImGui::InputText("Key password", keyPasswordBuf, sizeof(keyPasswordBuf), ImGuiInputTextFlags_Password);
  ImGui::SameLine();
  ImGui::TextDisabled("%s", storedHint(kKeyPwd));

  ImGui::Separator();

  if (ImGui::Button("Save")) {
    // Only overwrite secrets the dev typed something into this frame; preserves stored values
    // when the dev only edits, say, the keystore path. The keystore path itself always writes
    // (even when emptied, which Clears it via the empty-string branch).
    if (keystorePathBuf[0] != '\0') {
      octarine::secrets::Set(kKeyPath, keystorePathBuf);
      keystorePathLoaded = true;
    }
    if (storePasswordBuf[0] != '\0') {
      octarine::secrets::Set(kKeyStore, storePasswordBuf);
    }
    if (keyAliasBuf[0] != '\0') {
      octarine::secrets::Set(kKeyAlias, keyAliasBuf);
    }
    if (keyPasswordBuf[0] != '\0') {
      octarine::secrets::Set(kKeyPwd, keyPasswordBuf);
    }
    // Wipe the in-memory password buffers immediately after storing so they don't linger.
    std::memset(storePasswordBuf, 0, sizeof(storePasswordBuf));
    std::memset(keyPasswordBuf, 0, sizeof(keyPasswordBuf));
  }
  ImGui::SameLine();
  if (ImGui::Button("Clear all")) {
    octarine::secrets::Clear(kKeyPath);
    octarine::secrets::Clear(kKeyStore);
    octarine::secrets::Clear(kKeyAlias);
    octarine::secrets::Clear(kKeyPwd);
    std::memset(keystorePathBuf, 0, sizeof(keystorePathBuf));
    std::memset(storePasswordBuf, 0, sizeof(storePasswordBuf));
    std::memset(keyAliasBuf, 0, sizeof(keyAliasBuf));
    std::memset(keyPasswordBuf, 0, sizeof(keyPasswordBuf));
    keystorePathLoaded = false;
  }
  ImGui::SameLine();
  if (keystorePathLoaded) {
    ImGui::TextColored(ImVec4(0.4F, 1.0F, 0.4F, 1.0F), "Credentials saved.");
  }

  ImGui::End();
}

}  // namespace octarine::editor::panels

#endif  // OCTARINE_WITH_EDITOR
