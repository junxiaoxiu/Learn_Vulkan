#include "SDL2/SDL.h"
#include "SDL2/SDL_vulkan.h"

#include "vkl.hpp"

int main(int argc, char** argv) {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window = SDL_CreateWindow("sandbox", 
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          1024, 720,
                                          SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN);

    if (!window){
        SDL_Log("create window failed");
        exit(2);
    }

    bool shouldClose = false;
    SDL_Event event;

    vkl::Init();

    while(!shouldClose) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                shouldClose = true;
            }
        }
    }

    vkl::Quit();

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
