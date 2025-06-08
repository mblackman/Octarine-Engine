#pragma once

#include <spdlog/logger.h>

#include <memory>
#include <string>

class Logger {
  static std::shared_ptr<spdlog::logger> lua_logger_;

 public:
  static void Init();
  static void Log(const std::string& message);
  static void Error(const std::string& message);
  static void Warn(const std::string& message);
  static void Info(const std::string& message);
  static void LogLua(const std::string& message);
  static void ErrorLua(const std::string& message);
  static void WarnLua(const std::string& message);
  static void InfoLua(const std::string& message);
};