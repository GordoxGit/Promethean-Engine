#include "assets/AssetManager.h"
#include "core/LogSystem.h"

#include <SDL.h>

#ifndef __ANDROID__                         // Extensions SDL disponibles (desktop)
    #include <SDL_image.h>
    #include <SDL_mixer.h>
    #include <SDL_ttf.h>
#else                                        // --- Android : extensions absentes ---
    // Déclarations incomplètes pour compiler les pointeurs
    struct SDL_Texture;
    struct Mix_Chunk;
    struct TTF_Font;
#endif

#include <unordered_map>
#include <list>
#include <cassert>

namespace Promethean {

// ── Resource wrappers ───────────────────────────────────────────────────────
struct Texture { SDL_Texture* ptr; ~Texture() { if (ptr) SDL_DestroyTexture(ptr); } };
struct Sound   { Mix_Chunk*  ptr; ~Sound()   { if (ptr) /* Mix_FreeChunk absent */ ; } };
struct Font    { TTF_Font*   ptr; ~Font()    { if (ptr) /* TTF_CloseFont absent */ ; } };

// ── Internal implementation ─────────────────────────────────────────────────
struct AssetManager::Impl {
    explicit Impl(std::size_t cap) : capacity(cap) {}

    struct Entry {
        std::shared_ptr<void>              resource;
        std::list<std::string>::iterator   it;
    };

    std::size_t                                   capacity;
    std::unordered_map<std::string, Entry>        cache;
    std::list<std::string>                        lru;

    void Insert(const std::string& key, std::shared_ptr<void> res)
    {
        if (cache.size() >= capacity) {
            std::string oldKey = lru.back();      // copie sûre avant pop_back()
            lru.pop_back();
            cache.erase(oldKey);
        }
        lru.push_front(key);
        cache[key] = { res, lru.begin() };
    }
};

static std::string FontKey(const std::string& path, int size)
{
    return path + "#" + std::to_string(size);
}

// ── Ctor / Dtor ─────────────────────────────────────────────────────────────
AssetManager::AssetManager(std::size_t cacheSize)
    : m_impl(std::make_unique<Impl>(cacheSize)) {}

AssetManager::~AssetManager() = default;

// ── Public API ──────────────────────────────────────────────────────────────
std::shared_ptr<Texture> AssetManager::LoadTexture(const std::string& path)
{
    assert(!path.empty());
    const std::string key = "T:" + path;

    auto it = m_impl->cache.find(key);
    if (it != m_impl->cache.end()) {
        m_impl->lru.splice(m_impl->lru.begin(), m_impl->lru, it->second.it);
        LogSystem::Instance().Debug("AssetManager hit {}", path);
        return std::static_pointer_cast<Texture>(it->second.resource);
    }

    LogSystem::Instance().Debug("AssetManager miss {}", path);

#ifndef __ANDROID__
    SDL_Texture* raw = IMG_LoadTexture(nullptr, path.c_str());
    std::shared_ptr<Texture> tex;
    if (raw) tex.reset(new Texture{ raw });
    m_impl->Insert(key, std::static_pointer_cast<void>(tex));
    return tex;
#else
    LogSystem::Instance().Warn("LoadTexture unavailable on Android (SDL_image not built)");
    return nullptr;
#endif
}

std::shared_ptr<Sound> AssetManager::LoadSound(const std::string& path)
{
    assert(!path.empty());
    const std::string key = "S:" + path;

    auto it = m_impl->cache.find(key);
    if (it != m_impl->cache.end()) {
        m_impl->lru.splice(m_impl->lru.begin(), m_impl->lru, it->second.it);
        LogSystem::Instance().Debug("AssetManager hit {}", path);
        return std::static_pointer_cast<Sound>(it->second.resource);
    }

    LogSystem::Instance().Debug("AssetManager miss {}", path);

#ifndef __ANDROID__
    Mix_Chunk* raw = Mix_LoadWAV(path.c_str());
    std::shared_ptr<Sound> snd;
    if (raw) snd.reset(new Sound{ raw });
    m_impl->Insert(key, std::static_pointer_cast<void>(snd));
    return snd;
#else
    LogSystem::Instance().Warn("LoadSound unavailable on Android (SDL_mixer not built)");
    return nullptr;
#endif
}

std::shared_ptr<Font> AssetManager::LoadFont(const std::string& path, int size)
{
    assert(!path.empty());
    const std::string key = "F:" + FontKey(path, size);

    auto it = m_impl->cache.find(key);
    if (it != m_impl->cache.end()) {
        m_impl->lru.splice(m_impl->lru.begin(), m_impl->lru, it->second.it);
        LogSystem::Instance().Debug("AssetManager hit {}", key);
        return std::static_pointer_cast<Font>(it->second.resource);
    }

    LogSystem::Instance().Debug("AssetManager miss {}", key);

#ifndef __ANDROID__
    TTF_Font* raw = TTF_OpenFont(path.c_str(), size);
    std::shared_ptr<Font> font;
    if (raw) font.reset(new Font{ raw });
    m_impl->Insert(key, std::static_pointer_cast<void>(font));
    return font;
#else
    LogSystem::Instance().Warn("LoadFont unavailable on Android (SDL_ttf not built)");
    return nullptr;
#endif
}

} // namespace Promethean
