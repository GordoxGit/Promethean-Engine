#pragma once

namespace Promethean {

class WorldEditor;

/** Simple UI wrapper for the world editor. */
class EditorUI {
public:
    explicit EditorUI(WorldEditor& editor);

    /** Render the overlay UI. */
    void Render();

private:
    WorldEditor* m_editor{nullptr};
};

} // namespace Promethean
