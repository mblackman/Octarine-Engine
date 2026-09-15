#include "Logger.h"

#include <spdlog/spdlog.h>

#include <algorithm>
#include <cctype>

#if defined(__ANDROID__)
#include <spdlog/sinks/android_sink.h>
#else
#include <spdlog/sinks/stdout_color_sinks.h>
#endif

// iOS os_log sink is parked on the defer/ios branch pending an Apple Developer account.

std::shared_ptr<spdlog::logger> Logger::lua_logger_;
std::vector<std::string> Logger::history_;
std::mutex Logger::history_mutex_;
std::vector<Logger::ScriptError> Logger::script_errors_;
std::uint64_t Logger::script_error_sequence_ = 0;

namespace {

// Parses log level string, returning spdlog::level::n_levels on invalid input.
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
#if defined(__ANDROID__)
  // Android logcat sinks for engine and Lua messages.
  auto main_logger = spdlog::android_logger_mt("octarine", "Octarine");
  spdlog::set_default_logger(main_logger);
  lua_logger_ = spdlog::android_logger_mt("octarine-lua", "OctarineLua");
#else
  // Colored stdout sinks for desktop platforms.
  auto main_logger = spdlog::stdout_color_mt("octarine");
  spdlog::set_default_logger(main_logger);
  lua_logger_ = spdlog::stdout_color_mt("lua");
#endif

  spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");
  lua_logger_->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [lua] %v");

#ifdef OCTARINE_LOG_LEVEL_NAME
  SetLevel(OCTARINE_LOG_LEVEL_NAME);
#endif
}

void Logger::SetLevel(const std::string& level) {
  const auto parsed = ParseLevelName(level);
  if (parsed == spdlog::level::n_levels) {
    Logger::Warn("Unknown log level '" + level +
                 "' — expected trace|debug|info|warn|error|critical|off. Keeping current.");
    return;
  }
  spdlog::set_level(parsed);
  if (lua_logger_) {
    lua_logger_->set_level(parsed);
  }
}

void Logger::PushHistory(std::string entry) {
  // Cap history to prevent unbounded growth from per-frame logging.
  constexpr size_t kMaxHistory = 1000;
  std::lock_guard<std::mutex> lock(history_mutex_);
  history_.push_back(std::move(entry));
  if (history_.size() > kMaxHistory) {
    history_.erase(history_.begin());
  }
}

void Logger::Error(const std::string& message) {
  spdlog::error(message);
  PushHistory("[Error] " + message);
}

void Logger::Warn(const std::string& message) {
  spdlog::warn(message);
  PushHistory("[Warn] " + message);
}

void Logger::Info(const std::string& message) {
  spdlog::info(message);
  PushHistory("[Info] " + message);
}

void Logger::LogLua(const std::string& message) {
  lua_logger_->info(message);
  PushHistory(message);
}

void Logger::ErrorLua(const std::string& message) {
  lua_logger_->error(message);
  PushHistory("[Error] [Lua] " + message);
  // Cap toast history ring buffer.
  constexpr size_t kMaxScriptErrors = 8;
  std::lock_guard<std::mutex> lock(history_mutex_);
  script_errors_.push_back({++script_error_sequence_, message, std::chrono::steady_clock::now()});
  if (script_errors_.size() > kMaxScriptErrors) {
    script_errors_.erase(script_errors_.begin());
  }
}

void Logger::WarnLua(const std::string& message) {
  lua_logger_->warn(message);
  PushHistory("[Warn] [Lua] " + message);
}

void Logger::InfoLua(const std::string& message) {
  lua_logger_->info(message);
  PushHistory("[Info] [Lua] " + message);
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

std::vector<Logger::ScriptError> Logger::RecentScriptErrors() {
  std::lock_guard<std::mutex> lock(history_mutex_);
  return script_errors_;
}
