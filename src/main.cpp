#include "core/Engine.h"

#ifndef PROMETHEAN_ANDROID_CI
    #include <spdlog/spdlog.h>
    #include <SDL.h>
#endif

int main(int argc, char* argv[]) {
#ifdef PROMETHEAN_ANDROID_CI
    // Mode CI Android simplifié
    Promethean::Engine engine;
    if (!engine.Initialize()) {
        return 1;
    }
    engine.Shutdown();
    return 0;
#else
    Promethean::Engine engine;
    if (!engine.Initialize()) {
        spdlog::error("Failed to initialize engine");
        return 1;
    }

    engine.Run();

    engine.Shutdown();
    return 0;
#endif
}
