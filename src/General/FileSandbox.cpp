#include "General/FileSandbox.h"

#include <filesystem>
#include <fstream>
#include <system_error>

#include "General/Logger.h"

namespace file_sandbox {

std::string Resolve(const std::string& root, const std::string& relative) {
  if (root.empty()) {
    return {};
  }
  const std::filesystem::path inputPath(relative);
  if (inputPath.is_absolute()) {
    return {};
  }
  const std::filesystem::path basePath = std::filesystem::path(root).lexically_normal();
  const std::filesystem::path fullPath = (basePath / inputPath).lexically_normal();
  const auto rel = fullPath.lexically_relative(basePath);
  if (rel.empty() || *rel.begin() == "..") {
    return {};
  }
  return fullPath.string();
}

bool WriteFileAtomic(const std::string& fullPath, const std::string& contents) {
  const std::filesystem::path target(fullPath);
  std::error_code ec;
  if (target.has_parent_path()) {
    std::filesystem::create_directories(target.parent_path(), ec);
    if (ec) {
      Logger::Error("WriteFileAtomic: failed to create directories for " + fullPath + ": " + ec.message());
      return false;
    }
  }

  const std::filesystem::path tempPath = target.string() + ".tmp";
  {
    std::ofstream file(tempPath, std::ios::binary | std::ios::trunc);
    if (!file.is_open()) {
      Logger::Error("WriteFileAtomic: failed to open " + tempPath.string() + " for writing");
      return false;
    }
    file.write(contents.data(), static_cast<std::streamsize>(contents.size()));
    if (!file.good()) {
      Logger::Error("WriteFileAtomic: write failed for " + tempPath.string());
      file.close();
      std::filesystem::remove(tempPath, ec);
      return false;
    }
  }

  std::filesystem::rename(tempPath, target, ec);
  if (ec) {
    Logger::Error("WriteFileAtomic: rename to " + fullPath + " failed: " + ec.message());
    std::filesystem::remove(tempPath, ec);
    return false;
  }
  return true;
}

}  // namespace file_sandbox
