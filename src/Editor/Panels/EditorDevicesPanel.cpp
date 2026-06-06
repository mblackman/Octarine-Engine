#include "Editor/Panels/EditorDevicesPanel.h"

#ifdef OCTARINE_WITH_EDITOR

#include <cstdint>
#include <cstring>
#include <deque>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

// Socket includes for TCP Ping (matches DevListenServer.cpp conventions).
#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
using dev_socket_t = SOCKET;
constexpr dev_socket_t kDevInvalidSock = INVALID_SOCKET;
inline void DevCloseSocket(dev_socket_t s) { ::closesocket(s); }
#else
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
using dev_socket_t = int;
constexpr dev_socket_t kDevInvalidSock = -1;
inline void DevCloseSocket(dev_socket_t s) { ::close(s); }
#endif

#include "AssetManager/AssetManager.h"
#include "Deploy/Adb.h"
#include "Game/Game.h"
#include "Game/GameConfig.h"
#include "Process/Process.h"
#include "Project/ProjectIni.h"
#include "imgui.h"

namespace octarine::editor::panels {
namespace {

// ---- adb state ----
std::vector<octarine::deploy::AdbDevice> g_adbDevices;
std::vector<bool> g_adbSelected;
bool g_adbPresent = false;
std::string g_adbVersionStr;
double g_lastRefreshTime = -999.0;
static constexpr double kRefreshInterval = 2.0;

// Per-device action result badge (serial -> last result text).
std::unordered_map<std::string, std::string> g_actionFeedback;

// ---- Remote TCP state ----
struct RemoteTcpEntry {
  std::string host_port;
  bool selected = false;
  enum class PingStatus { Unknown, Reachable, Unreachable } ping_status = PingStatus::Unknown;
};
std::vector<RemoteTcpEntry> g_remoteEntries;
char g_addBuf[128] = "";

// ---- Logcat state (single active session) ----
std::optional<octarine::process::Process> g_logcatProcess;
std::string g_logcatSerial;
std::deque<std::string> g_logcatLines;
std::string g_logcatPartial;
bool g_logcatAutoScroll = true;
static constexpr std::size_t kLogcatLineCap = 2000;

// ---- APK install path (shared across adb rows) ----
char g_apkPathBuf[512] = "";

// -------- Helpers --------

void RefreshAdb() {
  if (auto ver = octarine::deploy::Adb::Detect()) {
    g_adbPresent = true;
    g_adbVersionStr = std::move(*ver);
  } else {
    g_adbPresent = false;
    g_adbVersionStr.clear();
    g_adbDevices.clear();
    g_adbSelected.clear();
    return;
  }
  g_adbDevices = octarine::deploy::Adb::ListDevices();
  // Grow/shrink the selection vector without clearing existing selections.
  g_adbSelected.resize(g_adbDevices.size(), false);
}

// Synchronous TCP connect probe with a short timeout (~500 ms). Returns true when the host:port
// accepts the connection. Brief UI stall on button press is acceptable for this action.
bool TcpPing(const std::string& host_port) {
  const auto colon = host_port.rfind(':');
  if (colon == std::string::npos || colon + 1 >= host_port.size()) return false;
  const std::string host = host_port.substr(0, colon);
  const std::string port_str = host_port.substr(colon + 1);

#ifdef _WIN32
  WSADATA wsa{};
  ::WSAStartup(MAKEWORD(2, 2), &wsa);
#endif

  addrinfo hints{};
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  addrinfo* res = nullptr;
  bool reachable = false;

  if (::getaddrinfo(host.c_str(), port_str.c_str(), &hints, &res) == 0 && res != nullptr) {
    dev_socket_t s = ::socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (s != kDevInvalidSock) {
#ifdef _WIN32
      DWORD ms = 500;
      ::setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<const char*>(&ms), sizeof(ms));
#else
      struct timeval tv {};
      tv.tv_usec = 500 * 1000;
      ::setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
#endif
      reachable = ::connect(s, res->ai_addr, static_cast<int>(res->ai_addrlen)) == 0;
      DevCloseSocket(s);
    }
    ::freeaddrinfo(res);
  }

#ifdef _WIN32
  ::WSACleanup();
#endif
  return reachable;
}

void StartLogcat(const std::string& serial) {
  if (g_logcatProcess) {
    g_logcatProcess->Kill();
    g_logcatProcess.reset();
  }
  g_logcatLines.clear();
  g_logcatPartial.clear();
  g_logcatSerial = serial;

  auto proc = octarine::deploy::Adb::Logcat(serial, {"Octarine:I", "*:S"});
  if (!proc) return;

  // Accumulate stdout into g_logcatLines, splitting on newlines. Callbacks invoked from
  // Pump() on the main thread so no synchronization needed.
  proc->OnStdout([](std::string_view chunk) {
    g_logcatPartial.append(chunk);
    std::size_t pos = 0;
    std::size_t nl;
    while ((nl = g_logcatPartial.find('\n', pos)) != std::string::npos) {
      g_logcatLines.push_back(g_logcatPartial.substr(pos, nl - pos));
      if (g_logcatLines.size() > kLogcatLineCap) g_logcatLines.pop_front();
      pos = nl + 1;
    }
    g_logcatPartial = g_logcatPartial.substr(pos);
  });
  g_logcatProcess = std::move(proc);
}

std::string GetProjectDir(Game* game) {
  auto* registry = game->GetRegistry();
  if (!registry->Get<GameConfig>().HasLoadedConfig()) return {};
  return registry->Get<AssetManager>().GetBasePath();
}

std::string GetPackageId(const std::string& project_dir) {
  if (project_dir.empty()) return {};
  auto ini = octarine::project::ProjectIni::Load(project_dir);
  return ini ? ini->package_id : std::string{};
}

// Inline small badge showing the last action result for a device keyed by `feedback_key`.
void ShowFeedback(const std::string& feedback_key) {
  auto it = g_actionFeedback.find(feedback_key);
  if (it != g_actionFeedback.end() && !it->second.empty()) {
    ImGui::SameLine();
    ImGui::TextDisabled("(%s)", it->second.c_str());
  }
}

}  // namespace

void DrawDevicesWindow(Game* game, bool* p_open) {
  // Per-frame: pump the logcat subprocess.
  if (g_logcatProcess && !g_logcatProcess->Pump()) {
    g_logcatProcess.reset();
  }

  // Auto-refresh adb device list every 2 s.
  if (ImGui::GetTime() - g_lastRefreshTime >= kRefreshInterval) {
    RefreshAdb();
    g_lastRefreshTime = ImGui::GetTime();
  }

  ImGui::SetNextWindowSize(ImVec2(740, 480), ImGuiCond_FirstUseEver);
  if (!ImGui::Begin("Devices", p_open)) {
    ImGui::End();
    return;
  }

  const std::string projectDir = GetProjectDir(game);
  const std::string packageId = GetPackageId(projectDir);
  auto& engineOptions = game->GetRegistry()->Get<GameConfig>().GetEngineOptions();

  // ---- Toolbar row ----
  if (ImGui::Button("Refresh")) {
    RefreshAdb();
    g_lastRefreshTime = ImGui::GetTime();
  }
  ImGui::SameLine();
  if (g_adbPresent) {
    ImGui::TextDisabled("%s", g_adbVersionStr.c_str());
  } else {
    ImGui::TextColored(ImVec4(1.0F, 0.85F, 0.4F, 1.0F), "adb not found");
    ImGui::SameLine();
    if (ImGui::SmallButton("Get Platform Tools")) {
      // No cross-platform OpenURL shim yet — copy the link to the clipboard.
      ImGui::SetClipboardText(octarine::deploy::kInstallPlatformToolsUrl);
    }
    ImGui::SameLine();
    ImGui::TextDisabled("(URL copied)");
  }

  // Count selected rows for multi-target Push Content.
  int numSelected = 0;
  for (bool s : g_adbSelected) {
    if (s) ++numSelected;
  }
  for (const auto& r : g_remoteEntries) {
    if (r.selected) ++numSelected;
  }

  if (numSelected > 0) {
    char pushLabel[64];
    std::snprintf(pushLabel, sizeof(pushLabel), "Push Content (%d selected)", numSelected);
    const float btnWidth = ImGui::CalcTextSize(pushLabel).x + ImGui::GetStyle().FramePadding.x * 2.0F;
    ImGui::SameLine(ImGui::GetContentRegionMax().x - btnWidth);
    ImGui::BeginDisabled(projectDir.empty());
    if (ImGui::Button(pushLabel)) {
      const std::string remote =
          "/sdcard/Android/data/" + (packageId.empty() ? "com.octarine.game" : packageId) + "/files/";
      for (std::size_t i = 0; i < g_adbDevices.size(); ++i) {
        if (!g_adbSelected[i] || !g_adbDevices[i].IsReady()) continue;
        const bool ok = octarine::deploy::Adb::Push(g_adbDevices[i].serial, projectDir, remote);
        g_actionFeedback[g_adbDevices[i].serial] = ok ? "pushed" : "push failed";
      }
    }
    ImGui::EndDisabled();
  }

  ImGui::Separator();

  // ---- Device table ----
  constexpr ImGuiTableFlags kFlags =
      ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchProp;
  if (ImGui::BeginTable("##DevicesTable", 4, kFlags)) {
    ImGui::TableSetupColumn("Device", ImGuiTableColumnFlags_WidthStretch, 0.35F);
    ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 90.0F);
    ImGui::TableSetupColumn("Status", ImGuiTableColumnFlags_WidthFixed, 90.0F);
    ImGui::TableSetupColumn("Actions", ImGuiTableColumnFlags_WidthStretch, 0.65F);
    ImGui::TableHeadersRow();

    // ---- Local row (always present) ----
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::TextUnformatted("This computer");
    ImGui::TableSetColumnIndex(1);
    ImGui::TextDisabled("Local");
    ImGui::TableSetColumnIndex(2);
    ImGui::TextColored(ImVec4(0.4F, 1.0F, 0.4F, 1.0F), "Available");
    ImGui::TableSetColumnIndex(3);
    {
      ImGui::BeginDisabled(!engineOptions.isPaused);
      if (ImGui::SmallButton("Play##local")) {
        if (!game->IsSceneRunning()) game->ReloadScene();
        engineOptions.isPaused = false;
      }
      ImGui::EndDisabled();
    }

    // ---- adb rows ----
    if (g_adbPresent && g_adbDevices.empty()) {
      ImGui::TableNextRow();
      ImGui::TableSetColumnIndex(0);
      ImGui::TextDisabled("No Android devices connected");
      ImGui::TableSetColumnIndex(1);
      ImGui::TextDisabled("Android");
    }

    for (std::size_t i = 0; i < g_adbDevices.size(); ++i) {
      const auto& dev = g_adbDevices[i];
      ImGui::TableNextRow();
      ImGui::PushID(static_cast<int>(i));

      // Device column: checkbox + label
      ImGui::TableSetColumnIndex(0);
      // std::vector<bool> returns a proxy, not an addressable bool — use a local.
      bool selRow = g_adbSelected[i];
      ImGui::Checkbox("##sel", &selRow);
      g_adbSelected[i] = selRow;
      ImGui::SameLine();
      if (dev.model.empty()) {
        ImGui::TextUnformatted(dev.serial.c_str());
      } else {
        ImGui::Text("%s (%s)", dev.model.c_str(), dev.serial.c_str());
      }

      ImGui::TableSetColumnIndex(1);
      ImGui::TextDisabled("Android");

      ImGui::TableSetColumnIndex(2);
      if (dev.IsReady()) {
        ImGui::TextColored(ImVec4(0.4F, 1.0F, 0.4F, 1.0F), "Ready");
      } else {
        ImGui::TextColored(ImVec4(1.0F, 0.85F, 0.4F, 1.0F), "%s", dev.state.c_str());
      }

      ImGui::TableSetColumnIndex(3);
      ImGui::BeginDisabled(!dev.IsReady());

      // Logcat toggle
      const bool logcatActive = g_logcatSerial == dev.serial && (g_logcatProcess || !g_logcatLines.empty());
      if (ImGui::SmallButton(logcatActive ? "Stop Logcat" : "Logcat")) {
        if (logcatActive) {
          if (g_logcatProcess) g_logcatProcess->Kill();
          g_logcatProcess.reset();
          g_logcatSerial.clear();
        } else {
          StartLogcat(dev.serial);
        }
      }
      ImGui::SameLine();

      // Install Runner — popup for APK path
      if (ImGui::SmallButton("Install")) {
        ImGui::OpenPopup("##install_popup");
      }
      if (ImGui::BeginPopup("##install_popup")) {
        ImGui::Text("APK path:");
        ImGui::SetNextItemWidth(340.0F);
        ImGui::InputText("##apkpath", g_apkPathBuf, sizeof(g_apkPathBuf));
        ImGui::SameLine();
        ImGui::BeginDisabled(g_apkPathBuf[0] == '\0');
        if (ImGui::Button("Install##do")) {
          const bool ok = octarine::deploy::Adb::Install(dev.serial, g_apkPathBuf);
          g_actionFeedback[dev.serial] = ok ? "installed" : "install failed";
          ImGui::CloseCurrentPopup();
        }
        ImGui::EndDisabled();
        ImGui::EndPopup();
      }
      ImGui::SameLine();

      // Push Content
      if (ImGui::SmallButton("Push Content")) {
        if (!projectDir.empty()) {
          const std::string remote =
              "/sdcard/Android/data/" + (packageId.empty() ? "com.octarine.game" : packageId) + "/files/";
          const bool ok = octarine::deploy::Adb::Push(dev.serial, projectDir, remote);
          g_actionFeedback[dev.serial] = ok ? "pushed" : "push failed";
        } else {
          g_actionFeedback[dev.serial] = "no project";
        }
      }
      ImGui::SameLine();

      // Launch (adb shell am start) — brief blocking call; am start exits quickly.
      if (ImGui::SmallButton("Launch")) {
        if (!packageId.empty()) {
          const std::string component = packageId + "/com.octarine.host.MainActivity";
          octarine::process::SpawnOptions opts;
          opts.argv = {"adb", "-s", dev.serial, "shell", "am", "start", "-n", component};
          auto proc = octarine::process::Process::Spawn(opts);
          if (proc) {
            proc->Wait();
            g_actionFeedback[dev.serial] = "launched";
          } else {
            g_actionFeedback[dev.serial] = "launch failed";
          }
        } else {
          g_actionFeedback[dev.serial] = "no package_id";
        }
      }

      ImGui::EndDisabled();
      ImGui::SameLine();

      // Disconnect (available regardless of IsReady)
      if (ImGui::SmallButton("Disconnect")) {
        const bool ok = octarine::deploy::Adb::Disconnect(dev.serial);
        g_actionFeedback[dev.serial] = ok ? "disconnected" : "failed";
      }

      ShowFeedback(dev.serial);
      ImGui::PopID();
    }

    // ---- Remote TCP rows ----
    for (std::size_t i = 0; i < g_remoteEntries.size(); ++i) {
      auto& entry = g_remoteEntries[i];
      const std::string feedbackKey = "tcp:" + entry.host_port;
      ImGui::TableNextRow();
      ImGui::PushID(static_cast<int>(1000 + i));

      ImGui::TableSetColumnIndex(0);
      ImGui::Checkbox("##sel", &entry.selected);
      ImGui::SameLine();
      ImGui::TextUnformatted(entry.host_port.c_str());

      ImGui::TableSetColumnIndex(1);
      ImGui::TextDisabled("Remote TCP");

      ImGui::TableSetColumnIndex(2);
      switch (entry.ping_status) {
        case RemoteTcpEntry::PingStatus::Reachable:
          ImGui::TextColored(ImVec4(0.4F, 1.0F, 0.4F, 1.0F), "Reachable");
          break;
        case RemoteTcpEntry::PingStatus::Unreachable:
          ImGui::TextColored(ImVec4(1.0F, 0.4F, 0.4F, 1.0F), "Unreachable");
          break;
        default:
          ImGui::TextDisabled("Unknown");
          break;
      }

      ImGui::TableSetColumnIndex(3);
      if (ImGui::SmallButton("Ping")) {
        entry.ping_status =
            TcpPing(entry.host_port) ? RemoteTcpEntry::PingStatus::Reachable : RemoteTcpEntry::PingStatus::Unreachable;
      }
      ImGui::SameLine();
      // Push Content and Eval require a dev-listen client (precursor to S13).
      ImGui::BeginDisabled(true);
      ImGui::SmallButton("Push Content");
      ImGui::SameLine();
      ImGui::SmallButton("Eval");
      ImGui::EndDisabled();
      ImGui::SameLine();
      if (ImGui::SmallButton("Remove")) {
        g_remoteEntries.erase(g_remoteEntries.begin() + static_cast<std::ptrdiff_t>(i));
        g_actionFeedback.erase(feedbackKey);
        ImGui::PopID();
        break;
      }
      ShowFeedback(feedbackKey);
      ImGui::PopID();
    }

    // ---- Add Remote TCP row ----
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::SetNextItemWidth(160.0F);
    ImGui::InputTextWithHint("##addtcp", "host:port", g_addBuf, sizeof(g_addBuf));
    ImGui::SameLine();
    const bool canAdd = g_addBuf[0] != '\0' && std::strchr(g_addBuf, ':') != nullptr;
    ImGui::BeginDisabled(!canAdd);
    if (ImGui::SmallButton("Add")) {
      g_remoteEntries.push_back({g_addBuf});
      std::memset(g_addBuf, 0, sizeof(g_addBuf));
    }
    ImGui::EndDisabled();

    ImGui::EndTable();
  }

  // ---- Logcat output (if active or has buffered lines) ----
  if (g_logcatProcess || !g_logcatLines.empty()) {
    ImGui::Separator();
    std::string logcatHeader = "Logcat";
    if (!g_logcatSerial.empty()) {
      logcatHeader += ": ";
      logcatHeader += g_logcatSerial;
    }
    ImGui::TextUnformatted(logcatHeader.c_str());
    ImGui::SameLine();
    if (g_logcatProcess) {
      ImGui::TextColored(ImVec4(0.4F, 1.0F, 0.4F, 1.0F), "(running)");
      ImGui::SameLine();
      if (ImGui::SmallButton("Stop")) {
        g_logcatProcess->Kill();
        g_logcatProcess.reset();
      }
    } else {
      ImGui::TextDisabled("(stopped)");
    }
    ImGui::SameLine();
    ImGui::Checkbox("Auto-scroll", &g_logcatAutoScroll);
    ImGui::SameLine();
    if (ImGui::SmallButton("Clear")) {
      g_logcatLines.clear();
      g_logcatPartial.clear();
    }

    ImGui::BeginChild("##LogcatScroll", ImVec2(0.0F, 160.0F), false, ImGuiWindowFlags_HorizontalScrollbar);
    for (const auto& line : g_logcatLines) {
      ImGui::TextUnformatted(line.c_str());
    }
    if (g_logcatAutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
      ImGui::SetScrollHereY(1.0F);
    }
    ImGui::EndChild();
  }

  ImGui::End();
}

}  // namespace octarine::editor::panels

#endif  // OCTARINE_WITH_EDITOR
