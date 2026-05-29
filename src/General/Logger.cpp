#include "Logger.h"

#include <spdlog/spdlog.h>

#include <algorithm>
#include <cctype>

#if defined(__ANDROID__)
#include <spdlog/sinks/android_sink.h>
#else
#include <spdlog/sinks/stdout_color_sinks.h>
#endif

std::shared_ptr<spdlog::logger> Logger::lua_logger_;
std::vector<std::string> Logger::history_;
std::mutex Logger::history_mutex_;

namespace {

// Hand-mapped level parse: spdlog::level::from_str silently returns `off` for unknown strings,
// which would erase the level instead of warning. We want a sentinel for "unknown" so callers
// can keep the current level and complain. Returns spdlog::level::n_levels on no match.
spdlog::level::level_enum ParseLevelName(std::string name) {
  std::transform(name.begin(), name.end(), name.begin(),
                 [](const unsigned char ch) { return static_cast<char>(std::tolower(ch)); });
  if (name == "trace") return spdlog::level::trace;
  if (name == "debug") return spdlog::level::debug;
  if (name == "info") return spdlog::level::info;
  if (name == "warn" || name == "warning") return spdlog::level::warn;
  if (name == "error" || name == "err") return spdlog::level::err;
  if (name == "critical" || name == "crit") return spdlog::level::critical;
  if (name == "off") return spdlog::level::off;
  return spdlog::level::n_levels;
}

}  // namespace

void Logger::Init() {
  // Replace spdlog's auto-default with a named logger we own, so platform sink + pattern + level
  // are applied consistently regardless of which spdlog default would have been picked.
#if defined(__ANDROID__)
  // android_sink routes through __android_log_write under a logcat tag. Engine messages land under
  // tag "Octarine", Lua messages under "OctarineLua" — both visible via `adb logcat -s Octarine
  // OctarineLua` and trivially filtered apart.
  auto main_logger = spdlog::android_logger_mt("octarine", "Octarine");
  spdlog::set_default_logger(main_logger);
  lua_logger_ = spdlog::android_logger_mt("octarine-lua", "OctarineLua");
#else
  // Desktop: keep the historical stdout-color sinks so the dev terminal experience is unchanged.
  auto main_logger = spdlog::stdout_color_mt("octarine");
  spdlog::set_default_logger(main_logger);
  lua_logger_ = spdlog::stdout_color_mt("lua");
#endif

  spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");
  lua_logger_->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [lua] %v");

#ifdef OCTARINE_LOG_LEVEL_NAME
  // Compile-time default: shipping builds default to `warn` (errors + warnings only), Debug builds
  // to `debug`, other configs to `info` (see CMakeLists.txt for the policy). A runtime
  // `LogLevel=` in config.ini overrides this via SetLevel() during Game::Setup.
  SetLevel(OCTARINE_LOG_LEVEL_NAME);
#endif
}

void Logger::SetLevel(const std::string& level) {
  const auto parsed = ParseLevelName(level);
  if (parsed == spdlog::level::n_levels) {
    Logger::Warn("Unknown log level '" + level + "' — expected trace|debug|info|warn|error|critical|off. Keeping current.");
    return;
  }
  spdlog::set_level(parsed);
  if (lua_logger_) {
    lua_logger_->set_level(parsed);
  }
}

void Logger::Error(const std::string& message) {
  spdlog::error(message);
  std::lock_guard<std::mutex> lock(history_mutex_);
  history_.push_back("[Error] " + message);
}

void Logger::Warn(const std::string& message) {
  spdlog::warn(message);
  std::lock_guard<std::mutex> lock(history_mutex_);
  history_.push_back("[Warn] " + message);
}

void Logger::Info(const std::string& message) {
  spdlog::info(message);
  std::lock_guard<std::mutex> lock(history_mutex_);
  history_.push_back("[Info] " + message);
}

void Logger::LogLua(const std::string& message) {
  lua_logger_->info(message);
  std::lock_guard<std::mutex> lock(history_mutex_);
  history_.push_back(message);
  if (history_.size() > 1000) {
    history_.erase(history_.begin());
  }
}

void Logger::ErrorLua(const std::string& message) {
  lua_logger_->error(message);
  std::lock_guard<std::mutex> lock(history_mutex_);
  history_.push_back("[Error] [Lua] " + message);
}

void Logger::WarnLua(const std::string& message) {
  lua_logger_->warn(message);
  std::lock_guard<std::mutex> lock(history_mutex_);
  history_.push_back("[Warn] [Lua] " + message);
}

void Logger::InfoLua(const std::string& message) {
  lua_logger_->info(message);
  std::lock_guard<std::mutex> lock(history_mutex_);
  history_.push_back("[Info] [Lua] " + message);
}

std::vector<std::string> Logger::GetHistory() {
  std::lock_guard<std::mutex> lock(history_mutex_);
  return history_;
}

void Logger::ForEachHistory(const std::function<void(const std::string&)>& callback) {
  std::lock_guard<std::mutex> lock(history_mutex_);
  for (const auto& line : history_) {
    callback(line);
  }
}

void Logger::ClearHistory() {
  std::lock_guard<std::mutex> lock(history_mutex_);
  history_.clear();
}
