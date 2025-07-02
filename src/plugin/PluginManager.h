#pragma once
#include <string>
#include <vector>

namespace Promethean {

struct PluginInfo {
    std::string path;
    bool active{false};
};

class PluginManager {
public:
    static PluginManager& Instance();

    bool LoadFromDirectory(const std::string& directory);
    const std::vector<PluginInfo>& GetPlugins() const { return m_plugins; }

private:
    PluginManager() = default;
    bool loadLua(const std::string& file);

    std::vector<PluginInfo> m_plugins;
};

} // namespace Promethean
