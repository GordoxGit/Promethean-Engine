#include "core/Engine.h"
#include <spdlog/spdlog.h>
#include <SDL.h>

int main(int argc, char* argv[]) {
    Promethean::Engine engine;
    if (!engine.Initialize()) {
        spdlog::error("Failed to initialize engine");
        return 1;
    }

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
            }
        }
        SDL_Delay(16); // Roughly 60fps
    }

    engine.Shutdown();
    return 0;
}
