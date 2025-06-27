# Repo Notes

- Version 3.12 of nlohmann-json still breaks on Android because `std::filesystem::path::u8string()` returns `std::string` when compiling with C++17.
- We define `JSON_HAS_FILESYSTEM 1` before including `<nlohmann/json.hpp>` and patch the generated `to_json.hpp` via `scripts/patch-json-android.sh` after `vcpkg install`.
- The script replaces the `std::u8string` block with a cross-platform version using `auto s = p.u8string();` and `std::string(reinterpret_cast<const char*>(s.data()), s.size())`. Any upstream changes in nlohmann-json may require updating the patch.
