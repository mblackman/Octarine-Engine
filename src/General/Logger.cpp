#include "Logger.h"

#include <spdlog/spdlog.h>

void Logger::Init() {
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");
}

void Logger::Log(const std::string& message) {
    spdlog::info(message);
}
void Logger::Error(const std::string& message) {
    spdlog::error(message);
}
void Logger::Warn(const std::string& message) {
    spdlog::warn(message);
}
void Logger::Info(const std::string& message) {
    spdlog::info(message);
}