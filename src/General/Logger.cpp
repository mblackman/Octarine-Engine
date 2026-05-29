#include "Logger.h"

#include <spdlog/spdlog.h>

#include <algorithm>
#include <cctype>

#if defined(__ANDROID__)
#include <spdlog/sinks/android_sink.h>
#else
#include <spdlog/sinks/stdout_color_sinks.h>
#endif

#if defined(__APPLE__)
#include <TargetConditionals.h>
#endif

#if defined(__APPLE__) && TARGET_OS_IPHONE
#include <os/log.h>

#include <mutex>
#include <spdlog/sinks/base_sink.h>

namespace {

// iOS app processes have no stdout/stderr attached, so spdlog's default color sink writes into the
// void. Route through Apple's os_log so messages show up in Console.app and `xcrun simctl spawn …
// log stream` under subsystem "com.octarine.engine".
template <typename Mutex>
class OsLogSink : public spdlog::sinks::base_sink<Mutex> {
 public:
  OsLogSink(const char* subsystem, const char* category) : log_(os_log_create(subsystem, category)) {}

 protected:
  void sink_it_(const spdlog::details::log_msg& msg) override {
    spdlog::memory_buf_t formatted;
    spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
    std::string out(formatted.data(), formatted.size());
    while (!out.empty() && (out.back() == '\n' || out.back() == '\r')) out.pop_back();
    os_log_type_t type = OS_LOG_TYPE_DEFAULT;
    switch (msg.level) {
      case spdlog::level::trace:
      case spdlog::level::debug:
        type = OS_LOG_TYPE_DEBUG;
        break;
      case spdlog::level::info:
        type = OS_LOG_TYPE_INFO;
        break;
      case spdlog::level::warn:
        type = OS_LOG_TYPE_DEFAULT;
        break;
      case spdlog::level::err:
        type = OS_LOG_TYPE_ERROR;
        break;
      case spdlog::level::critical:
        type = OS_LOG_TYPE_FAULT;
        break;
      default:
        break;
    }
    os_log_with_type(log_, type, "%{public}s", out.c_str());
  }
  void flush_() override {}

 private:
  os_log_t log_;
};

using OsLogSinkMt = OsLogSink<std::mutex>;

}  // namespace
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
#elif defined(__APPLE__) && TARGET_OS_IPHONE
  // iOS: stdout goes nowhere, so route through os_log. Categories "Octarine"/"OctarineLua" mirror
  // the Android tags — filter in Console.app or `log stream --predicate 'subsystem ==
  // "com.octarine.engine"'`.
  auto main_logger = std::make_shared<spdlog::logger>(
      "octarine", std::make_shared<OsLogSinkMt>("com.octarine.engine", "Octarine"));
  spdlog::register_logger(main_logger);
  spdlog::set_default_logger(main_logger);
  lua_logger_ = std::make_shared<spdlog::logger>(
      "octarine-lua", std::make_shared<OsLogSinkMt>("com.octarine.engine", "OctarineLua"));
  spdlog::register_logger(lua_logger_);
#else
  // Desktop (incl. macOS): keep the historical stdout-color sinks so the dev terminal experience is
  // unchanged.
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
