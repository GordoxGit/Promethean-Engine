#pragma once

namespace Promethean {

class WorldEditor;

/** Simple UI wrapper for the world editor. */
class EditorUI {
public:
    explicit EditorUI(WorldEditor& editor);

    /** Render the overlay UI. */
    void Render();
    void ShowPlugins();

private:
    WorldEditor* m_editor{nullptr};
};

} // namespace Promethean
