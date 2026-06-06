#include "Editor/HotPusher.h"

#ifdef OCTARINE_WITH_EDITOR

#include <algorithm>
#include <fstream>
#include <iterator>

#include "Deploy/Adb.h"
#include "Dev/DevListenClient.h"
#include "Project/ProjectIni.h"

namespace octarine::editor {

void HotPusher::Tick(double imgui_time, const std::filesystem::path& project_dir) {
  if (project_dir != project_dir_) {
    UpdateProjectDir(project_dir);
  }

  if (project_dir_.empty()) return;
  if (imgui_time - last_poll_time_ < kPollInterval) return;
  last_poll_time_ = imgui_time;

  auto changed = CollectChangedFiles();
  if (changed.empty()) return;

  for (auto& target : targets_) {
    if (!target.auto_push) continue;
    target.last_bytes = PushFilesToTarget(target, changed, true);
    target.last_push_time = imgui_time;
  }
}

void HotPusher::PushAll(const std::string& address, const std::filesystem::path& project_dir) {
  if (project_dir != project_dir_) {
    UpdateProjectDir(project_dir);
  }
  auto* target = FindTarget(address);
  if (!target) return;
  target->last_bytes = PushAllFilesToTarget(*target);
}

void HotPusher::UpsertTarget(const std::string& address, PushTarget::Kind kind) {
  for (auto& t : targets_) {
    if (t.address == address) {
      t.kind = kind;
      return;
    }
  }
  PushTarget t;
  t.address = address;
  t.kind = kind;
  targets_.push_back(std::move(t));
}

void HotPusher::RemoveTarget(const std::string& address) {
  targets_.erase(std::remove_if(targets_.begin(), targets_.end(),
                                [&address](const PushTarget& t) { return t.address == address; }),
                 targets_.end());
}

PushTarget* HotPusher::FindTarget(const std::string& address) {
  for (auto& t : targets_) {
    if (t.address == address) return &t;
  }
  return nullptr;
}

void HotPusher::UpdateProjectDir(const std::filesystem::path& dir) {
  project_dir_ = dir;
  manifest_.clear();
  package_id_.clear();
  if (dir.empty()) return;
  auto ini = octarine::project::ProjectIni::Load(dir);
  if (ini) package_id_ = ini->package_id;
}

std::vector<std::filesystem::path> HotPusher::CollectChangedFiles() {
  std::vector<std::filesystem::path> changed;
  if (project_dir_.empty()) return changed;

  std::error_code ec;
  std::unordered_map<std::string, bool> seen;

  for (const auto& entry : std::filesystem::recursive_directory_iterator(
           project_dir_, std::filesystem::directory_options::skip_permission_denied, ec)) {
    if (ec) {
      ec.clear();
      continue;
    }
    if (!entry.is_regular_file(ec)) {
      ec.clear();
      continue;
    }
    // Skip hidden directories
    bool hidden = false;
    for (const auto& part : entry.path().lexically_relative(project_dir_)) {
      const std::string s = part.string();
      if (!s.empty() && s[0] == '.') {
        hidden = true;
        break;
      }
    }
    if (hidden) continue;

    const std::string key = entry.path().string();
    seen[key] = true;

    const auto mtime = entry.last_write_time(ec);
    if (ec) {
      ec.clear();
      continue;
    }
    auto it = manifest_.find(key);
    if (it == manifest_.end() || it->second != mtime) {
      manifest_[key] = mtime;
      changed.push_back(entry.path());
    }
  }

  // Remove entries for deleted files
  for (auto it = manifest_.begin(); it != manifest_.end();) {
    if (seen.find(it->first) == seen.end()) {
      it = manifest_.erase(it);
    } else {
      ++it;
    }
  }

  return changed;
}

std::vector<char> HotPusher::ReadFile(const std::filesystem::path& path) {
  std::ifstream f(path, std::ios::binary);
  if (!f) return {};
  return std::vector<char>(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());
}

std::size_t HotPusher::PushFilesToTarget(PushTarget& target, const std::vector<std::filesystem::path>& files,
                                         bool reload_after) {
  // Sort: non-.lua assets first, .lua scripts second
  std::vector<std::filesystem::path> sorted = files;
  std::stable_sort(sorted.begin(), sorted.end(), [](const std::filesystem::path& a, const std::filesystem::path& b) {
    const bool a_lua = a.extension() == ".lua";
    const bool b_lua = b.extension() == ".lua";
    if (a_lua != b_lua) return !a_lua;
    return false;
  });

  std::size_t total_bytes = 0;

  for (const auto& abs_path : sorted) {
    const auto rel_path = abs_path.lexically_relative(project_dir_);
    const std::string rel_str = rel_path.generic_string();
    const bool is_lua = abs_path.extension() == ".lua";

    auto content = ReadFile(abs_path);
    if (content.empty() && std::filesystem::file_size(abs_path) > 0) continue;

    if (target.kind == PushTarget::Kind::DevListen) {
      dev::ClientResult result;
      if (is_lua) {
        result = dev::DevListenClient::PushScript(target.address, rel_str, content);
      } else {
        result = dev::DevListenClient::PushAsset(target.address, rel_str, content);
      }
      target.last_ok = result.ok;
      target.last_message = result.message;
      if (result.ok) {
        total_bytes += content.size();
      }
    } else {
      // Adb
      const std::string remote =
          "/sdcard/Android/data/" + (package_id_.empty() ? "com.octarine.game" : package_id_) + "/files/" + rel_str;
      const bool ok = octarine::deploy::Adb::Push(target.address, abs_path.string(), remote);
      target.last_ok = ok;
      target.last_message = ok ? "" : "adb push failed";
      if (ok) {
        total_bytes += content.size();
      }
    }
  }

  if (target.kind == PushTarget::Kind::DevListen && reload_after && !sorted.empty()) {
    auto result = dev::DevListenClient::ReloadScene(target.address);
    if (!result.ok) {
      target.last_ok = false;
      target.last_message = result.message;
    }
  }

  return total_bytes;
}

std::size_t HotPusher::PushAllFilesToTarget(PushTarget& target) {
  if (project_dir_.empty()) return 0;

  std::vector<std::filesystem::path> all_files;
  std::error_code ec;
  for (const auto& entry : std::filesystem::recursive_directory_iterator(
           project_dir_, std::filesystem::directory_options::skip_permission_denied, ec)) {
    if (ec) {
      ec.clear();
      continue;
    }
    if (!entry.is_regular_file(ec)) {
      ec.clear();
      continue;
    }
    bool hidden = false;
    for (const auto& part : entry.path().lexically_relative(project_dir_)) {
      const std::string s = part.string();
      if (!s.empty() && s[0] == '.') {
        hidden = true;
        break;
      }
    }
    if (hidden) continue;
    all_files.push_back(entry.path());
  }

  return PushFilesToTarget(target, all_files, true);
}

}  // namespace octarine::editor

#endif  // OCTARINE_WITH_EDITOR
