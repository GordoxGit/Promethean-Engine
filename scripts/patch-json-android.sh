#!/usr/bin/env bash
set -euo pipefail

FILE="vcpkg_installed/arm64-android/include/nlohmann/detail/conversions/to_json.hpp"

if [ ! -f "$FILE" ]; then
    echo "File $FILE not found" >&2
    exit 1
fi

if grep -q "__cpp_lib_char8_t" "$FILE"; then
    echo "nlohmann::json patch already applied"
    exit 0
fi

# Replace the incompatible line with conditional code
sed -i 's|const std::u8string s = p.u8string();|\
#if defined(__cpp_lib_char8_t)\
const std::u8string u8s = p.u8string();\
j = std::string(reinterpret_cast<const char*>(u8s.data()), u8s.size());\
#else\
const std::string s = p.u8string();\
j = s;\
#endif|' "$FILE"

# Verify patch
if grep -q "__cpp_lib_char8_t" "$FILE"; then
    echo "✅ nlohmann::json patched for Android char8_t"
else
    echo "❌ Failed to patch nlohmann::json" >&2
    exit 1
fi
