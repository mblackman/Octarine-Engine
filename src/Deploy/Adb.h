#pragma once

// Adb — thin wrapper around the Android Platform Tools `adb` CLI, used by the editor's Devices
// panel + hot-push deploy path. Every call shells out through octarine::process::Process; nothing
// here links the Android SDK. Blocking calls (ListDevices/Connect/Install/Push/Reverse/Detect)
// spawn-and-Wait; Logcat hands back a live Process the caller owns and pumps for streaming output.
//
// Lives in octarine_core (alongside Process/) so both the editor and a headless CLI can reach it.

#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "Process/Process.h"

namespace octarine::deploy {

// One row of `adb devices -l`. `state` is adb's own word ("device", "offline", "unauthorized",
// "no permissions", ...); only "device" means usable, which IsReady() reflects.
struct AdbDevice {
  std::string serial;   // "emulator-5554" or "192.168.1.5:5555"
  std::string state;    // adb connection state
  std::string model;    // model:<...> from -l (underscores kept), empty when absent
  std::string product;  // product:<...> from -l, empty when absent

  [[nodiscard]] bool IsReady() const { return state == "device"; }
};

// Shown by the UI when Detect() reports adb is missing.
inline constexpr const char* kInstallPlatformToolsUrl = "https://developer.android.com/tools/releases/platform-tools";

class Adb {
 public:
  // PATH-probe via `adb version`. Returns adb's first version line on success, nullopt when adb is
  // absent / not runnable (UI then points the user at kInstallPlatformToolsUrl).
  static std::optional<std::string> Detect();

  // `adb devices -l` parsed into rows. Empty on adb-missing / command failure / no devices.
  static std::vector<AdbDevice> ListDevices();

  // Pure parser for `adb devices -l` stdout — exposed so it can be unit-tested without a device.
  static std::vector<AdbDevice> ParseDevices(std::string_view devicesOutput);

  // `adb connect <hostPort>` / `adb disconnect <hostPort>`. True when adb reports success.
  static bool Connect(const std::string& hostPort);
  static bool Disconnect(const std::string& hostPort);

  // `adb -s <serial> install [-r] <apkPath>`. Empty serial targets the single attached device.
  static bool Install(const std::string& serial, const std::string& apkPath, bool replace = true);

  // `adb -s <serial> push <local> <remote>`.
  static bool Push(const std::string& serial, const std::string& local, const std::string& remote);

  // `adb -s <serial> reverse tcp:<devicePort> tcp:<hostPort>` — forwards a device-side port back to
  // the host so an on-device build can reach the host's --dev-listen server over localhost.
  static bool Reverse(const std::string& serial, int devicePort, int hostPort);

  // `adb -s <serial> logcat <tags...>` — returns a LIVE process for the caller to Pump() for
  // streaming logs (and Kill() when done). nullopt if adb could not be spawned. `tags` are passed
  // verbatim (e.g. {"Octarine:I", "*:S"} to isolate the engine tag).
  static std::optional<octarine::process::Process> Logcat(const std::string& serial,
                                                          const std::vector<std::string>& tags = {});
};

}  // namespace octarine::deploy
