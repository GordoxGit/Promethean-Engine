#include "plugin/PluginManager.h"
#include "scripting/ScriptingManager.h"
#include <filesystem>

namespace fs = std::filesystem;

namespace Promethean {

PluginManager& PluginManager::Instance() {
    static PluginManager inst;
    return inst;
}

bool PluginManager::LoadFromDirectory(const std::string& directory) {
    bool ok = true;
    for(const auto& entry : fs::directory_iterator(directory)) {
        if(entry.is_regular_file()) {
            auto path = entry.path();
            auto ext = path.extension().string();
            if(ext == ".lua") {
                ok &= loadLua(path.string());
            }
        }
    }
    return ok;
}

bool PluginManager::loadLua(const std::string& file) {
    bool result = pe::scripting::ScriptingManager::Instance().RunScript(file);
    m_plugins.push_back({file, result});
    return result;
}

} // namespace Promethean
