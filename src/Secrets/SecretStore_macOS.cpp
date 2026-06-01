#include "Secrets/SecretStore.h"

#if defined(__APPLE__)

#include <CoreFoundation/CoreFoundation.h>
#include <Security/Security.h>

#include <string>

#include "General/Logger.h"

namespace octarine::secrets {
namespace {
// CFSTR expands to a __builtin reinterpret_cast that Clang rejects in constexpr context.
// The macro itself produces a process-lifetime constant CFString, so a static const is fine.
static const CFStringRef kService = CFSTR("com.octarine.engine.secrets");

CFStringRef MakeAccount(std::string_view key) {
  return CFStringCreateWithBytes(nullptr, reinterpret_cast<const UInt8*>(key.data()), static_cast<CFIndex>(key.size()),
                                 kCFStringEncodingUTF8,
                                 /*isExternalRepresentation=*/false);
}

CFDataRef MakeData(std::string_view value) {
  return CFDataCreate(nullptr, reinterpret_cast<const UInt8*>(value.data()), static_cast<CFIndex>(value.size()));
}

CFMutableDictionaryRef BaseQuery(CFStringRef account) {
  CFMutableDictionaryRef q =
      CFDictionaryCreateMutable(nullptr, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
  CFDictionarySetValue(q, kSecClass, kSecClassGenericPassword);
  CFDictionarySetValue(q, kSecAttrService, kService);
  CFDictionarySetValue(q, kSecAttrAccount, account);
  return q;
}
}  // namespace

bool IsAvailable() { return true; }

bool Set(std::string_view key, std::string_view value) {
  CFStringRef account = MakeAccount(key);
  CFDataRef data = MakeData(value);

  // Try update first; fall back to add. Avoids the "duplicate item" failure mode if the
  // entry already exists.
  CFMutableDictionaryRef query = BaseQuery(account);
  CFMutableDictionaryRef attrs =
      CFDictionaryCreateMutable(nullptr, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
  CFDictionarySetValue(attrs, kSecValueData, data);

  OSStatus s = SecItemUpdate(query, attrs);
  if (s == errSecItemNotFound) {
    CFMutableDictionaryRef add = BaseQuery(account);
    CFDictionarySetValue(add, kSecValueData, data);
    s = SecItemAdd(add, nullptr);
    CFRelease(add);
  }

  CFRelease(attrs);
  CFRelease(query);
  CFRelease(data);
  CFRelease(account);

  if (s != errSecSuccess) {
    Logger::Error("SecretStore: keychain Set failed (OSStatus=" + std::to_string(s) + ")");
    return false;
  }
  return true;
}

std::optional<std::string> Get(std::string_view key) {
  CFStringRef account = MakeAccount(key);
  CFMutableDictionaryRef query = BaseQuery(account);
  CFDictionarySetValue(query, kSecReturnData, kCFBooleanTrue);
  CFDictionarySetValue(query, kSecMatchLimit, kSecMatchLimitOne);

  CFTypeRef out = nullptr;
  OSStatus s = SecItemCopyMatching(query, &out);

  CFRelease(query);
  CFRelease(account);

  if (s == errSecItemNotFound) {
    return std::nullopt;
  }
  if (s != errSecSuccess || !out) {
    Logger::Error("SecretStore: keychain Get failed (OSStatus=" + std::to_string(s) + ")");
    if (out) CFRelease(out);
    return std::nullopt;
  }

  CFDataRef data = reinterpret_cast<CFDataRef>(out);
  const std::string result(reinterpret_cast<const char*>(CFDataGetBytePtr(data)),
                           static_cast<std::size_t>(CFDataGetLength(data)));
  CFRelease(out);
  return result;
}

bool Clear(std::string_view key) {
  CFStringRef account = MakeAccount(key);
  CFMutableDictionaryRef query = BaseQuery(account);
  OSStatus s = SecItemDelete(query);
  CFRelease(query);
  CFRelease(account);
  // Removing a missing item is success per our contract.
  return s == errSecSuccess || s == errSecItemNotFound;
}
}  // namespace octarine::secrets

#endif  // __APPLE__
