#!/usr/bin/env bash
set -euo pipefail

FILE="vcpkg_installed/arm64-android/include/nlohmann/detail/conversions/to_json.hpp"

if [ ! -f "$FILE" ]; then
    echo "File $FILE not found" >&2
    exit 1
fi

if grep -q "reinterpret_cast<const char*" "$FILE"; then
    echo "nlohmann::json patch already applied"
    exit 0
fi

perl -0pi -e '
    s/#ifdef JSON_HAS_CPP_20\n//s;
    s/\n#else\n\s*j = p\.u8string\(\); \/\/ returns std::string in C\+\+17\n#endif//s;
    s/const std::u8string\s+[a-zA-Z0-9_]*\s*=\s*p\.u8string\(\);/auto s = p.u8string();/s;
    s/j = std::string\(s.begin\(\), s.end\(\)\);/j = std::string(reinterpret_cast<const char*>(s.data()), s.size());/s;
' "$FILE"

if grep -q "reinterpret_cast<const char*" "$FILE"; then
    echo "✅ nlohmann::json patched for Android char8_t"
else
    echo "❌ Failed to patch nlohmann::json" >&2
    exit 1
fi
