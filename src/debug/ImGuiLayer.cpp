#include "debug/ImGuiLayer.h"
#include "core/SettingsManager.h"
#include "audio/AudioEngine.h"
#include <SDL_mixer.h>

#ifdef PROMETHEAN_WITH_IMGUI
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#endif

namespace Promethean {

#ifdef PROMETHEAN_WITH_IMGUI
static ImGuiLayer* s_instance = nullptr;
#endif

void ImGuiLayer::init(SDL_Window* window, void* glContext)
{
#ifdef PROMETHEAN_WITH_IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init();
    ImGui::StyleColorsDark();
    s_instance = this;
#endif
}

void ImGuiLayer::begin()
{
#ifdef PROMETHEAN_WITH_IMGUI
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    if(showAudioWindow) {
        if(ImGui::Begin("Audio")) {
            auto& settings = SettingsManager::Instance().audio();
            if(ImGui::SliderInt("BGM", &settings.bgm, 0, MIX_MAX_VOLUME)) {
                AudioEngine::Instance().setBusVolume(AudioBus::BGM, settings.bgm);
                SettingsManager::Instance().markDirty();
            }
            if(ImGui::Checkbox("Mute BGM", &settings.muteBgm)) {
                AudioEngine::Instance().muteBus(AudioBus::BGM, settings.muteBgm);
                SettingsManager::Instance().markDirty();
            }
            if(ImGui::SliderInt("SFX", &settings.sfx, 0, MIX_MAX_VOLUME)) {
                AudioEngine::Instance().setBusVolume(AudioBus::SFX, settings.sfx);
                SettingsManager::Instance().markDirty();
            }
            if(ImGui::Checkbox("Mute SFX", &settings.muteSfx)) {
                AudioEngine::Instance().muteBus(AudioBus::SFX, settings.muteSfx);
                SettingsManager::Instance().markDirty();
            }
            if(ImGui::SliderInt("UI", &settings.ui, 0, MIX_MAX_VOLUME)) {
                AudioEngine::Instance().setBusVolume(AudioBus::UI, settings.ui);
                SettingsManager::Instance().markDirty();
            }
            if(ImGui::Checkbox("Mute UI", &settings.muteUi)) {
                AudioEngine::Instance().muteBus(AudioBus::UI, settings.muteUi);
                SettingsManager::Instance().markDirty();
            }
        }
        ImGui::End();
    }
#endif
}

void ImGuiLayer::end()
{
#ifdef PROMETHEAN_WITH_IMGUI
    auto& mgr = SettingsManager::Instance();
    if(mgr.isDirty()) {
        mgr.save();
    }
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
}

void ImGuiLayer::handleEvent(const SDL_Event& event)
{
#ifdef PROMETHEAN_WITH_IMGUI
    ImGui_ImplSDL2_ProcessEvent(&event);
#endif
}

} // namespace Promethean
