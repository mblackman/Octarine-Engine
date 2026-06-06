#include "Deploy/Adb.h"

#include <sstream>
#include <utility>

#include "General/Logger.h"

namespace octarine::deploy {
namespace {
using octarine::process::Process;
using octarine::process::SpawnOptions;

// Builds an adb argv, prefixing `-s <serial>` when a serial is given (empty = the single attached
// device). `tail` is the subcommand + its args.
std::vector<std::string> AdbArgv(const std::string& serial, std::vector<std::string> tail) {
  std::vector<std::string> argv;
  argv.reserve(tail.size() + 3);
  argv.emplace_back("adb");
  if (!serial.empty()) {
    argv.emplace_back("-s");
    argv.push_back(serial);
  }
  for (auto& a : tail) argv.push_back(std::move(a));
  return argv;
}

// Spawns `adb <args>` to completion, capturing stdout (and stderr) into `*out` when non-null.
// Returns the exit code, or -1 when adb could not be spawned (not on PATH).
int RunAdb(std::vector<std::string> argv, std::string* out) {
  SpawnOptions opts;
  opts.argv = std::move(argv);
  auto proc = Process::Spawn(opts);
  if (!proc) {
    Logger::Warn("Adb: failed to spawn 'adb' (is Android Platform Tools on PATH?)");
    return -1;
  }
  if (out != nullptr) {
    proc->OnStdout([out](std::string_view chunk) { out->append(chunk); });
    proc->OnStderr([out](std::string_view chunk) { out->append(chunk); });
  }
  return proc->Wait();
}

// Pulls the value of a `key:value` token (e.g. "model:Pixel_5") out of a -l detail line. Empty if
// the key isn't present.
std::string DetailField(const std::string& line, const std::string& key) {
  const std::string needle = key + ":";
  std::size_t pos = line.find(needle);
  if (pos == std::string::npos) return {};
  pos += needle.size();
  const std::size_t end = line.find_first_of(" \t", pos);
  return line.substr(pos, end == std::string::npos ? std::string::npos : end - pos);
}
}  // namespace

std::vector<AdbDevice> Adb::ParseDevices(std::string_view devicesOutput) {
  std::vector<AdbDevice> devices;
  std::istringstream stream{std::string(devicesOutput)};
  std::string line;
  while (std::getline(stream, line)) {
    // Trim a trailing CR so CRLF capture (Windows adb) parses identically.
    if (!line.empty() && line.back() == '\r') line.pop_back();
    if (line.empty()) continue;
    // Skip the header and the daemon chatter adb sometimes prepends.
    if (line.rfind("List of devices", 0) == 0) continue;
    if (line.rfind("* daemon", 0) == 0) continue;

    std::istringstream cols{line};
    AdbDevice dev;
    cols >> dev.serial >> dev.state;
    if (dev.serial.empty() || dev.state.empty()) continue;
    dev.model = DetailField(line, "model");
    dev.product = DetailField(line, "product");
    devices.push_back(std::move(dev));
  }
  return devices;
}

std::optional<std::string> Adb::Detect() {
  std::string out;
  if (RunAdb({"adb", "version"}, &out) != 0) return std::nullopt;
  // First non-empty line, e.g. "Android Debug Bridge version 1.0.41".
  std::istringstream stream{out};
  std::string line;
  while (std::getline(stream, line)) {
    if (!line.empty() && line.back() == '\r') line.pop_back();
    if (!line.empty()) return line;
  }
  return out.empty() ? std::nullopt : std::optional<std::string>{out};
}

std::vector<AdbDevice> Adb::ListDevices() {
  std::string out;
  if (RunAdb({"adb", "devices", "-l"}, &out) != 0) return {};
  return ParseDevices(out);
}

bool Adb::Connect(const std::string& hostPort) {
  std::string out;
  // adb connect exits 0 even for some failures on older clients; gate on the success phrasing too.
  return RunAdb({"adb", "connect", hostPort}, &out) == 0 && out.find("connected") != std::string::npos;
}

bool Adb::Disconnect(const std::string& hostPort) { return RunAdb({"adb", "disconnect", hostPort}, nullptr) == 0; }

bool Adb::Install(const std::string& serial, const std::string& apkPath, bool replace) {
  std::vector<std::string> tail{"install"};
  if (replace) tail.emplace_back("-r");
  tail.push_back(apkPath);
  std::string out;
  return RunAdb(AdbArgv(serial, std::move(tail)), &out) == 0 && out.find("Success") != std::string::npos;
}

bool Adb::Push(const std::string& serial, const std::string& local, const std::string& remote) {
  return RunAdb(AdbArgv(serial, {"push", local, remote}), nullptr) == 0;
}

bool Adb::Reverse(const std::string& serial, int devicePort, int hostPort) {
  const std::string deviceSpec = "tcp:" + std::to_string(devicePort);
  const std::string hostSpec = "tcp:" + std::to_string(hostPort);
  return RunAdb(AdbArgv(serial, {"reverse", deviceSpec, hostSpec}), nullptr) == 0;
}

std::optional<Process> Adb::Logcat(const std::string& serial, const std::vector<std::string>& tags) {
  std::vector<std::string> tail{"logcat"};
  for (const auto& t : tags) tail.push_back(t);
  SpawnOptions opts;
  opts.argv = AdbArgv(serial, std::move(tail));
  return Process::Spawn(opts);
}

}  // namespace octarine::deploy
