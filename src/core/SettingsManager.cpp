#include "core/SettingsManager.h"
#include <filesystem>

namespace fs = std::filesystem;

namespace Promethean {

SettingsManager& SettingsManager::Instance() {
    static SettingsManager inst;
    return inst;
}

bool SettingsManager::load(const std::string& path) {
    m_path = path;
    if(fs::exists(path)) {
        m_audio = AudioSettings::loadSettings(path);
        return true;
    }
    return false;
}

bool SettingsManager::save() {
    if(m_path.empty()) return false;
    return m_audio.saveSettings(m_path);
}

} // namespace Promethean

