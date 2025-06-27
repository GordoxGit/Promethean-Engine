# Repo Notes

- The workaround for nlohmann-json's `char8_t` issue was removed because version 3.12 supposedly works on all platforms.
- Android builds with NDK r25 still fail due to `std::filesystem::path::u8string()` returning `std::string`.
- A conditional patch is reintroduced in `scripts/patch-json-android.sh` and applied during the Android CI job after vcpkg installation.
