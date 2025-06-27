#include "debug/RuntimeOverlay.h"
#include "core/LogSystem.h"
#include "audio/AudioEngine.h"
#include "core/SettingsManager.h"
#include "renderer/BatchRenderer.h"
#include <SDL.h>
#include <vector>

#ifdef PE_IMGUI_ENABLED
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#endif

namespace pe::debug {

static bool s_visible = false;
static bool s_prevF1 = false;
#ifdef PE_IMGUI_ENABLED
static bool s_initialized = false;
static uint64_t s_lastTime = 0;
#endif
static std::vector<PanelCallback> s_panels;
static pe::ecs::Registry* s_registry = nullptr;

void Init(ImGuiContext* shared)
{
#ifdef PE_IMGUI_ENABLED
    if (s_initialized)
        return;
    if (shared)
        ImGui::SetCurrentContext(shared);
    else {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
    }
    SDL_Window* win = SDL_GL_GetCurrentWindow();
    void* gl = SDL_GL_GetCurrentContext();
    if (!win || !gl) {
        LogSystem::Instance().Warn("RuntimeOverlay: no GL context");
        return;
    }
    ImGui_ImplSDL2_InitForOpenGL(win, gl);
    ImGui_ImplOpenGL3_Init();
    ImGui::StyleColorsDark();
    s_initialized = true;
    s_lastTime = SDL_GetPerformanceCounter();
    LogSystem::Instance().Info("RuntimeOverlay initialized");
#else
    (void)shared;
#endif
}

void RenderOverlay()
{
    bool f1 = SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_F1];
    if (f1 && !s_prevF1)
        s_visible = !s_visible;
    s_prevF1 = f1;
#ifdef PE_IMGUI_ENABLED
    if (!s_initialized)
        return;
#endif
    if (!s_visible)
        return;
#ifdef PE_IMGUI_ENABLED
    uint64_t now = SDL_GetPerformanceCounter();
    float dt = float(now - s_lastTime) / SDL_GetPerformanceFrequency();
    s_lastTime = now;
    float fps = dt > 0.f ? 1.f / dt : 0.f;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Debug Overlay");
    ImGui::Text("FPS: %.1f", fps);
    ImGui::Text("Frame: %.2f ms", dt * 1000.f);
    ImGui::Text("Draw calls: %d", BatchRenderer::GetDrawCallCount());
    size_t ent = s_registry ? s_registry->active() : 0;
    ImGui::Text("Entities: %zu", ent);
    ImGui::Text("VRAM approx: %.1f MB", 0.0f);
    if (ImGui::CollapsingHeader("Audio")) {
        int bgm = AudioEngine::Instance().getBusVolume(AudioBus::BGM);
        if (ImGui::SliderInt("BGM", &bgm, 0, MIX_MAX_VOLUME))
            AudioEngine::Instance().setBusVolume(AudioBus::BGM, bgm);
        int sfx = AudioEngine::Instance().getBusVolume(AudioBus::SFX);
        if (ImGui::SliderInt("SFX", &sfx, 0, MIX_MAX_VOLUME))
            AudioEngine::Instance().setBusVolume(AudioBus::SFX, sfx);
        int ui = AudioEngine::Instance().getBusVolume(AudioBus::UI);
        if (ImGui::SliderInt("UI", &ui, 0, MIX_MAX_VOLUME))
            AudioEngine::Instance().setBusVolume(AudioBus::UI, ui);
        static bool mute = false;
        if (ImGui::Checkbox("Mute All", &mute)) {
            AudioEngine::Instance().muteBus(AudioBus::BGM, mute);
            AudioEngine::Instance().muteBus(AudioBus::SFX, mute);
            AudioEngine::Instance().muteBus(AudioBus::UI, mute);
        }
    }
    ImGui::End();
#endif

    for(auto& cb : s_panels)
        cb();

#ifdef PE_IMGUI_ENABLED
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
}

void Shutdown()
{
#ifdef PE_IMGUI_ENABLED
    if (!s_initialized)
        return;
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    s_initialized = false;
#endif
}

void RegisterPanel(PanelCallback cb)
{
    s_panels.push_back(std::move(cb));
}

void SetRegistry(pe::ecs::Registry* reg)
{
    s_registry = reg;
}

#ifdef TESTING
bool IsVisible() { return s_visible; }
void ForceSetVolume(int bgm, int sfx, int ui)
{
#ifdef PE_IMGUI_ENABLED
    AudioEngine::Instance().setBusVolume(AudioBus::BGM, bgm);
    AudioEngine::Instance().setBusVolume(AudioBus::SFX, sfx);
    AudioEngine::Instance().setBusVolume(AudioBus::UI, ui);
#else
    (void)bgm; (void)sfx; (void)ui;
#endif
}

void Toggle()
{
    s_visible = !s_visible;
}
#endif

} // namespace pe::debug
