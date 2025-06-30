#include "editor/EditorUI.h"
#include "editor/WorldEditor.h"

namespace Promethean {

EditorUI::EditorUI(WorldEditor& editor)
    : m_editor(&editor) {}

void EditorUI::Render()
{
    (void)m_editor; // UI would interact with the editor
}

} // namespace Promethean
