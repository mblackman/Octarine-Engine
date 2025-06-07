#include "Logger.h"

#include <spdlog/spdlog.h>
#include "spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<spdlog::logger> Logger::lua_logger_;

void Logger::Init() {
  lua_logger_ = spdlog::stdout_color_mt("lua");
  spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");
  lua_logger_->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [lua] %v");
}

void Logger::Log(const std::string& message) { spdlog::info(message); }
void Logger::Error(const std::string& message) { spdlog::error(message); }
void Logger::Warn(const std::string& message) { spdlog::warn(message); }
void Logger::Info(const std::string& message) { spdlog::info(message); }
void Logger::LogLua(const std::string& message) { lua_logger_->info(message); }
void Logger::ErrorLua(const std::string& message) { lua_logger_->error(message); }
void Logger::WarnLua(const std::string& message) { lua_logger_->warn(message); }
void Logger::InfoLua(const std::string& message) { lua_logger_->info(message); }
