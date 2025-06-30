#pragma once
#include "core/State.h"
#include "editor/WorldEditor.h"
#include "editor/EditorUI.h"

namespace Promethean {

/** Game state running the world editor. */
class EditorState : public State {
public:
    EditorState();

    void HandleEvent(const SDL_Event& ev) override;
    void Update(float dt) override;
    void Render(BatchRenderer& renderer) override;

    WorldEditor& GetEditor() { return m_editor; }

private:
    WorldEditor m_editor;
    EditorUI    m_ui;
};

} // namespace Promethean
