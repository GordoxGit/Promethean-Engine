# Repo Notes

- Version 3.12 of nlohmann-json still breaks on Android because `std::filesystem::path::u8string()` returns `std::string` when compiling with C++17.
- We define `JSON_HAS_FILESYSTEM 1` before including `<nlohmann/json.hpp>` and patch the generated `to_json.hpp` via `scripts/patch-json-android.sh` after `vcpkg install`.
