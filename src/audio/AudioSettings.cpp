#include "audio/AudioSettings.h"
#include <fstream>

namespace Promethean {

bool AudioSettings::saveSettings(const std::string& path) const {
    try {
        std::ofstream ofs(path);
        if(!ofs.good()) return false;
        nlohmann::json j = *this;
        ofs << j.dump(4);
        return true;
    } catch(...) {
        return false;
    }
}

AudioSettings AudioSettings::loadSettings(const std::string& path) {
    AudioSettings s;
    std::ifstream ifs(path);
    if(ifs.good()) {
        nlohmann::json j; ifs >> j;
        s = j.get<AudioSettings>();
    }
    return s;
}

} // namespace Promethean
