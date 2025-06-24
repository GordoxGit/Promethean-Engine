#include <gtest/gtest.h>
#include <SDL.h>

int main(int argc, char **argv) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    ::testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();

    SDL_Quit();
    return ret;
}
