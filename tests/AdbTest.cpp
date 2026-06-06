// Unit checks for the adb wrapper's pure parser (Adb::ParseDevices). gtest-free; failed-check
// count is the exit code. Mirrors the style of ProcessTest / DevListenServerTest. Registered as
// AdbTest in ctest. No device or adb binary is required — only the stdout-parsing logic is tested.

#include <iostream>
#include <string>

#include "Deploy/Adb.h"

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
}  // namespace

int main() {
  using octarine::deploy::Adb;
  using octarine::deploy::AdbDevice;

  {
    // Typical `adb devices -l`: header, a USB device, a wifi/tcp device, and an offline one.
    const std::string out =
        "List of devices attached\n"
        "emulator-5554          device product:sdk_gphone_x86 model:Android_SDK_built_for_x86 "
        "device:generic_x86 transport_id:1\n"
        "192.168.1.5:5555       device product:redfin model:Pixel_5 device:redfin transport_id:2\n"
        "0123456789ABCDEF       offline\n";
    const std::vector<AdbDevice> devices = Adb::ParseDevices(out);

    Check(devices.size() == 3, "parses one row per device, skipping the header");
    if (devices.size() == 3) {
      Check(devices[0].serial == "emulator-5554", "row 0 serial");
      Check(devices[0].state == "device", "row 0 state");
      Check(devices[0].model == "Android_SDK_built_for_x86", "row 0 model field extracted");
      Check(devices[0].IsReady(), "row 0 IsReady() for state=device");

      Check(devices[1].serial == "192.168.1.5:5555", "row 1 tcp serial kept intact");
      Check(devices[1].model == "Pixel_5", "row 1 model field extracted");
      Check(devices[1].product == "redfin", "row 1 product field extracted");

      Check(devices[2].serial == "0123456789ABCDEF", "row 2 serial");
      Check(devices[2].state == "offline", "row 2 state=offline");
      Check(!devices[2].IsReady(), "row 2 not IsReady() when offline");
      Check(devices[2].model.empty(), "row 2 model empty when -l detail absent");
    }
  }

  {
    // Empty / no-devices output yields no rows (header only, plus daemon chatter).
    const std::string out =
        "* daemon not running; starting now at tcp:5037\n"
        "* daemon started successfully\n"
        "List of devices attached\n";
    Check(Adb::ParseDevices(out).empty(), "no devices -> empty, daemon lines skipped");
  }

  {
    // CRLF-terminated capture (Windows adb) parses identically.
    const std::string out = "List of devices attached\r\nemulator-5554\tdevice\r\n";
    const std::vector<AdbDevice> devices = Adb::ParseDevices(out);
    Check(devices.size() == 1 && devices[0].serial == "emulator-5554" && devices[0].state == "device",
          "CRLF + tab-separated row parses, trailing CR trimmed");
  }

  return g_failures;
}
