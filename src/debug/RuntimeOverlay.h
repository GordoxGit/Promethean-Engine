#pragma once
#if defined(PROMETHEAN_WITH_IMGUI) && !defined(HEADLESS_GL)
#  define PE_IMGUI_ENABLED 1
#endif

#include <functional>

struct ImGuiContext;
namespace pe { namespace ecs { class Registry; } }

namespace pe::debug {
    using PanelCallback = std::function<void()>;
    void RegisterPanel(PanelCallback cb);
    void SetRegistry(pe::ecs::Registry* reg);
    void Init(ImGuiContext* optionalShared = nullptr);
    void RenderOverlay();
    void Shutdown();
#ifdef TESTING
    bool IsVisible();
    void ForceSetVolume(int bgm, int sfx, int ui);
    void Toggle();
#endif
}
