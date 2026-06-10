#pragma once

#include <spdlog/logger.h>

#include <chrono>
#include <cstdint>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

class Logger {
 public:
  // One captured Lua script error for the editor's on-screen toast. `sequence` increases
  // monotonically across the run so consumers can tell new errors from re-reads; `when` lets
  // the toast expire entries by age.
  struct ScriptError {
    std::uint64_t sequence{};
    std::string message;
    std::chrono::steady_clock::time_point when{};
  };

 private:
  static std::shared_ptr<spdlog::logger> lua_logger_;
  static std::vector<std::string> history_;
  static std::mutex history_mutex_;
  static std::vector<ScriptError> script_errors_;
  static std::uint64_t script_error_sequence_;

 public:
  static void Init();
  // Apply a runtime log level override (trace|debug|info|warn|error|critical|off). Case-insensitive;
  // unknown values log a warning and leave the current level intact. Typically called once from
  // Game::Setup after config.ini's `LogLevel=` is parsed, on top of the compile-time default.
  static void SetLevel(const std::string& level);
  static void Error(const std::string& message);
  static void Warn(const std::string& message);
  static void Info(const std::string& message);
  static void LogLua(const std::string& message);
  static void ErrorLua(const std::string& message);
  static void WarnLua(const std::string& message);
  static void InfoLua(const std::string& message);

  static std::vector<std::string> GetHistory();
  static void ForEachHistory(const std::function<void(const std::string&)>& callback);
  static void ClearHistory();

  // Snapshot of the most recent script errors (newest last), capped to a small ring. Fed by
  // ErrorLua — i.e. every runtime Lua failure that log-and-continues — and consumed by the
  // editor-build error toast so script failures stay visible when the console is hidden.
  static std::vector<ScriptError> RecentScriptErrors();
};