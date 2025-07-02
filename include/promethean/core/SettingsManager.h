#pragma once
#include "audio/AudioSettings.h"
#include <string>

namespace Promethean {

class SettingsManager {
public:
    static SettingsManager& Instance();

    bool load(const std::string& path);
    bool save();

    AudioSettings& audio() { return m_audio; }
    const AudioSettings& audio() const { return m_audio; }

    void markDirty() { m_dirty = true; }
    bool isDirty() const { return m_dirty; }

private:
    SettingsManager() = default;
    AudioSettings m_audio;
    bool m_dirty{false};
    std::string m_path;
};

} // namespace Promethean
