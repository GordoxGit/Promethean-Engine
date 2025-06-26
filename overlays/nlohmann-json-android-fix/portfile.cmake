vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO nlohmann/json
    REF v3.12.0
    SHA512 6cc1e86261f8fac21cc17a33da3b6b3c3cd5c116755651642af3c9e99bb3538fd42c1bd50397a77c8fb6821bc62d90e6b91bcdde77a78f58f2416c62fc53b97d
    HEAD_REF master
)

if(VCPKG_TARGET_IS_ANDROID)
    file(READ "${SOURCE_PATH}/single_include/nlohmann/json.hpp" _json_hpp)
    string(REPLACE "#ifdef JSON_HAS_CPP_20\n    p = std_fs::path(std::u8string_view(reinterpret_cast<const char8_t*>(s.data()), s.size()));\n#else\n    p = std_fs::u8path(s); // accepts UTF-8 encoded std::string in C++17, deprecated in C++20\n#endif" "#if JSON_ANDROID_CHAR8T_HACK\n    p = std_fs::u8path(s);\n#elif defined(JSON_HAS_CPP_20)\n    p = std_fs::path(std::u8string_view(reinterpret_cast<const char8_t*>(s.data()), s.size()));\n#else\n    p = std_fs::u8path(s); // accepts UTF-8 encoded std::string in C++17, deprecated in C++20\n#endif" _json_hpp)
    string(REPLACE "#ifdef JSON_HAS_CPP_20\n    const std::u8string s = p.u8string();\n    j = std::string(s.begin(), s.end());\n#else\n    j = p.u8string(); // returns std::string in C++17\n#endif" "#if JSON_ANDROID_CHAR8T_HACK\n    j = p.u8string();\n#elif defined(JSON_HAS_CPP_20)\n    const std::u8string s = p.u8string();\n    j = std::string(s.begin(), s.end());\n#else\n    j = p.u8string(); // returns std::string in C++17\n#endif" _json_hpp)
    file(WRITE "${SOURCE_PATH}/single_include/nlohmann/json.hpp" "${_json_hpp}")
endif()

if(NOT DEFINED nlohmann-json_IMPLICIT_CONVERSIONS)
    set(nlohmann-json_IMPLICIT_CONVERSIONS ON)
endif()

vcpkg_check_features(OUT_FEATURE_OPTIONS FEATURE_OPTIONS
FEATURES
    "diagnostics"           JSON_Diagnostics
)

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS ${FEATURE_OPTIONS}
        -DJSON_Install=ON
        -DJSON_MultipleHeaders=ON
        -DJSON_BuildTests=OFF
        -DJSON_ImplicitConversions=${nlohmann-json_IMPLICIT_CONVERSIONS}
)
vcpkg_cmake_install()
vcpkg_cmake_config_fixup(PACKAGE_NAME "nlohmann_json" CONFIG_PATH "share/cmake/nlohmann_json")
vcpkg_fixup_pkgconfig()

vcpkg_replace_string(
    "${CURRENT_PACKAGES_DIR}/share/nlohmann_json/nlohmann_jsonTargets.cmake"
    "{_IMPORT_PREFIX}/nlohmann_json.natvis"
    "{_IMPORT_PREFIX}/share/nlohmann_json/nlohmann_json.natvis"
    IGNORE_UNCHANGED
)
if(EXISTS "${CURRENT_PACKAGES_DIR}/nlohmann_json.natvis")
    file(RENAME
        "${CURRENT_PACKAGES_DIR}/nlohmann_json.natvis"
        "${CURRENT_PACKAGES_DIR}/share/nlohmann_json/nlohmann_json.natvis"
    )
endif()

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug")

# Handle copyright
file(INSTALL "${SOURCE_PATH}/LICENSE.MIT" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)

# Handle usage
file(COPY "${CMAKE_CURRENT_LIST_DIR}/usage" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}")
