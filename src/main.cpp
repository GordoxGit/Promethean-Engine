#include "core/Engine.h"

#ifndef PROMETHEAN_ANDROID_CI
    #include <spdlog/spdlog.h>
    #include <SDL.h>
    #include <string>
    #include "assets/TileMap.h"
#endif

int main(int argc, char* argv[]) {
#ifndef PROMETHEAN_ANDROID_CI
    std::string mapFile;
    for(int i=1;i<argc;i++)
    {
        std::string a(argv[i]);
        if(a.rfind("--map=",0)==0)
            mapFile = a.substr(6);
    }
#endif
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

    if(!mapFile.empty())
    {
        auto map = Promethean::LoadTileMap(mapFile);
        (void)map;
    }

    engine.Run();

    engine.Shutdown();
    return 0;
#endif
}
