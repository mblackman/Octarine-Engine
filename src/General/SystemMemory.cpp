#include "General/SystemMemory.h"

#if defined(_WIN32)

// PSAPI_VERSION 2 routes GetProcessMemoryInfo to K32GetProcessMemoryInfo in kernel32, so no extra
// psapi.lib link is needed.
#define PSAPI_VERSION 2
#define WIN32_LEAN_AND_MEAN
// clang-format off
#include <windows.h>
#include <psapi.h>  // requires windows.h first
// clang-format on

std::uint64_t SystemMemory::GetResidentBytes() {
  PROCESS_MEMORY_COUNTERS counters{};
  if (GetProcessMemoryInfo(GetCurrentProcess(), &counters, sizeof(counters)) == 0) {
    return 0;
  }
  return static_cast<std::uint64_t>(counters.WorkingSetSize);
}

#elif defined(__APPLE__)

#include <mach/mach.h>

std::uint64_t SystemMemory::GetResidentBytes() {
  mach_task_basic_info info{};
  mach_msg_type_number_t count = MACH_TASK_BASIC_INFO_COUNT;
  if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO, reinterpret_cast<task_info_t>(&info), &count) != KERN_SUCCESS) {
    return 0;
  }
  return static_cast<std::uint64_t>(info.resident_size);
}

#else  // Linux / Android

#include <unistd.h>

#include <cstdio>
#include <cstdlib>

std::uint64_t SystemMemory::GetResidentBytes() {
  std::FILE* file = std::fopen("/proc/self/statm", "r");
  if (file == nullptr) {
    return 0;
  }
  // statm fields are in pages: size resident shared ...
  char buf[64] = {};
  const bool readOk = std::fgets(buf, sizeof(buf), file) != nullptr;
  std::fclose(file);
  if (!readOk) return 0;
  char* end1 = nullptr;
  char* end2 = nullptr;
  std::strtoull(buf, &end1, 10);
  if (end1 == buf) return 0;
  const unsigned long long residentPages = std::strtoull(end1, &end2, 10);
  if (end2 == end1) return 0;
  const long pageSize = sysconf(_SC_PAGESIZE);
  if (pageSize <= 0) {
    return 0;
  }
  return static_cast<std::uint64_t>(residentPages) * static_cast<std::uint64_t>(pageSize);
}

#endif
