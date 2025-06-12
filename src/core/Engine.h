#pragma once
#include <SDL.h>

class Engine {
public:
    bool initialize();
    void run();
    void shutdown();
private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    bool running = false;
};
