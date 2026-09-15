#pragma once

#include <string>

// Path validation and sandboxed file I/O utilities.
namespace file_sandbox {

// Resolves relative path within root. Rejects absolute paths and directory traversal (../).
std::string Resolve(const std::string& root, const std::string& relative);

// Writes file atomically via temporary sibling file and rename. Creates missing parent directories.
bool WriteFileAtomic(const std::string& fullPath, const std::string& contents);

}  // namespace file_sandbox
