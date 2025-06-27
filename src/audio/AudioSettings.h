#pragma once
#define JSON_HAS_FILESYSTEM 1
#include <nlohmann/json.hpp>
#include <SDL_mixer.h>
#include <string>

namespace Promethean {

struct AudioSettings {
    int bgm = MIX_MAX_VOLUME;
    int sfx = MIX_MAX_VOLUME;
    int ui  = MIX_MAX_VOLUME;
    bool muteBgm = false;
    bool muteSfx = false;
    bool muteUi  = false;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(AudioSettings, bgm, sfx, ui, muteBgm, muteSfx, muteUi)

    bool saveSettings(const std::string& path) const;
    static AudioSettings loadSettings(const std::string& path);
};

} // namespace Promethean
