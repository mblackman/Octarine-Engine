#pragma once

#ifndef OCTARINE_SHIPPED

#include <cstdint>
#include <string>
#include <vector>

namespace octarine::dev {

struct ClientResult {
  bool ok = false;
  std::string message;
};

class DevListenClient {
 public:
  static ClientResult Ping(const std::string& host_port);
  static ClientResult PushScript(const std::string& host_port, const std::string& relative_path,
                                 const std::vector<char>& content);
  static ClientResult PushAsset(const std::string& host_port, const std::string& relative_path,
                                const std::vector<char>& content);
  static ClientResult ReloadScene(const std::string& host_port);
  static ClientResult EvalLua(const std::string& host_port, const std::string& src);
};

}  // namespace octarine::dev
#endif  // !OCTARINE_SHIPPED
