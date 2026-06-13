#include "General/LuaEscape.h"

#include <cstddef>
#include <cstdio>

std::string EscapeLua(const std::string& s) {
  constexpr unsigned char kFirstPrintable = ' ';  // 0x20
  constexpr unsigned char kDel = '\x7f';
  constexpr std::size_t kHexBufSize = 8;  // "\\xNN" + nul with room to spare
  std::string out;
  out.reserve(s.size());
  for (const char raw : s) {
    const auto c = static_cast<unsigned char>(raw);
    if (c == '\\' || c == '"') {
      out.push_back('\\');
      out.push_back(raw);
    } else if (c < kFirstPrintable || c == kDel) {
      char buf[kHexBufSize];
      std::snprintf(buf, sizeof(buf), "\\x%02x", c);
      out.append(buf);
    } else {
      out.push_back(raw);
    }
  }
  return out;
}
