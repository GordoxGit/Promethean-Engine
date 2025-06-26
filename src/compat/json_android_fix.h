#pragma once
#ifdef __ANDROID__
#include <nlohmann/json.hpp>
#include <filesystem>

// Disable C++20 detection for json in this translation unit
#undef JSON_HAS_CPP_20
#define JSON_HAS_CPP_20 0

namespace nlohmann {
    template<>
    struct adl_serializer<std::filesystem::path> {
        static void to_json(json& j, const std::filesystem::path& p) {
            const auto tmp = p.u8string();
            j = std::u8string(reinterpret_cast<const char8_t*>(tmp.data()), tmp.size());
        }
        static void from_json(const json& j, std::filesystem::path& p) {
            p = std::filesystem::path(j.get<std::string>());
        }
    };
}
#endif // __ANDROID__
