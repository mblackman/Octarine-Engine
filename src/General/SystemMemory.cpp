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

std::uint64_t SystemMemory::GetResidentBytes() {
  std::FILE* file = std::fopen("/proc/self/statm", "r");
  if (file == nullptr) {
    return 0;
  }
  // statm fields are in pages: size resident shared ...
  unsigned long long sizePages = 0;
  unsigned long long residentPages = 0;
  const int parsed = std::fscanf(file, "%llu %llu", &sizePages, &residentPages);
  std::fclose(file);
  if (parsed != 2) {
    return 0;
  }
  const long pageSize = sysconf(_SC_PAGESIZE);
  if (pageSize <= 0) {
    return 0;
  }
  return static_cast<std::uint64_t>(residentPages) * static_cast<std::uint64_t>(pageSize);
}

#endif
