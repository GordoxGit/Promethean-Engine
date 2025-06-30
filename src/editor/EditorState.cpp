#include "editor/EditorState.h"

namespace Promethean {

EditorState::EditorState()
    : m_editor(), m_ui(m_editor) {}

void EditorState::HandleEvent(const SDL_Event&)
{
    // Input handling would go here
}

void EditorState::Update(float)
{
    // Editor update logic
}

void EditorState::Render(BatchRenderer&)
{
    m_ui.Render();
}

} // namespace Promethean
