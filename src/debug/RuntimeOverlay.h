#pragma once
#if defined(PROMETHEAN_WITH_IMGUI) && !defined(HEADLESS_GL)
#  define PE_IMGUI_ENABLED 1
#endif

struct ImGuiContext;

namespace pe::debug {
    void Init(ImGuiContext* optionalShared = nullptr);
    void RenderOverlay();
    void Shutdown();
#ifdef TESTING
    bool IsVisible();
    void ForceSetVolume(int bgm, int sfx, int ui);
    void Toggle();
#endif
}
