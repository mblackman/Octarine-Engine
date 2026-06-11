#pragma once

#include <cstdint>

namespace SystemMemory {
// Resident memory of the current process in bytes (Windows working set, POSIX RSS), or 0 if the
// platform query fails. Cheap (single syscall / small proc read) but not free — callers that poll
// per frame should throttle.
std::uint64_t GetResidentBytes();
}  // namespace SystemMemory
