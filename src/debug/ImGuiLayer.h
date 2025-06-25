#pragma once
#include <SDL.h>

namespace Promethean {

class ImGuiLayer {
public:
    void init(SDL_Window* window, void* glContext);
    void begin();
    void end();
    void handleEvent(const SDL_Event& event);

    bool showAudioWindow{false};
};

} // namespace Promethean
