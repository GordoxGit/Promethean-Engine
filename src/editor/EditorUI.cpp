#include "editor/EditorUI.h"
#include "editor/WorldEditor.h"
#include "plugin/PluginManager.h"
#include <iostream>

namespace Promethean {

EditorUI::EditorUI(WorldEditor& editor)
    : m_editor(&editor) {}

void EditorUI::Render()
{
    (void)m_editor; // UI would interact with the editor
}

void EditorUI::ShowPlugins()
{
    auto& pm = PluginManager::Instance();
    for(const auto& p : pm.GetPlugins()) {
        std::cout << "Plugin: " << p.path << (p.active ? " [active]" : " [error]") << "\n";
    }
}

} // namespace Promethean
