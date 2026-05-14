#include "Logger.h"

#include <spdlog/spdlog.h>

#include "spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<spdlog::logger> Logger::lua_logger_;
std::vector<std::string> Logger::history_;
std::mutex Logger::history_mutex_;

void Logger::Init() {
  lua_logger_ = spdlog::stdout_color_mt("lua");
  spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");
  lua_logger_->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [lua] %v");
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
