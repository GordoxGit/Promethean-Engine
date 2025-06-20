#pragma once

#include <memory>
#include <string>

namespace Promethean {

class Texture;
class Sound;
class Font;

/**
 * @brief Centralized asset manager with LRU caching.
 */
class AssetManager {
public:
    /**
     * @brief Construct the manager with a maximum cache size.
     * @param cacheSize Number of entries (all types combined).
     */
    explicit AssetManager(std::size_t cacheSize);
    ~AssetManager();

    /** Load a texture from disk or cache. */
    std::shared_ptr<Texture> LoadTexture(const std::string& path);
    /** Load a sound effect. */
    std::shared_ptr<Sound>   LoadSound  (const std::string& path);
    /** Load a font at the given size. */
    std::shared_ptr<Font>    LoadFont   (const std::string& path, int size);

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl; ///< Pimpl hiding LRU implementation.
};

} // namespace Promethean
