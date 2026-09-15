#pragma once

#include <string>

// Escapes a string for embedding inside a Lua double-quoted string literal.
std::string EscapeLua(const std::string& s);
