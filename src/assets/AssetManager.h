#pragma once

#include <string>
#include <unordered_map>
#include <cstdint>

using TextureHandle = uint32_t;

/**
 * @brief Minimal asset manager used for loading textures.
 *
 * This is a placeholder implementation meant for unit tests.
 */
class AssetManager {
public:
    static AssetManager& Instance();

    /** Load a texture and return its handle. */
    TextureHandle LoadTexture(const std::string& path);

#ifdef TESTING
    void Reset();
#endif

private:
    AssetManager() = default;
    std::unordered_map<std::string, TextureHandle> m_loaded;
    TextureHandle m_nextHandle{1};
};

inline AssetManager& AssetManager::Instance()
{
    static AssetManager instance;
    return instance;
}

inline TextureHandle AssetManager::LoadTexture(const std::string& path)
{
    auto it = m_loaded.find(path);
    if(it != m_loaded.end())
        return it->second;
    TextureHandle handle = m_nextHandle++;
    m_loaded[path] = handle;
    return handle;
}

#ifdef TESTING
inline void AssetManager::Reset()
{
    m_loaded.clear();
    m_nextHandle = 1;
}
#endif

