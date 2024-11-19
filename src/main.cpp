#include "SDL2/SDL.h"
#include "SDL2/SDL_vulkan.h"

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "vkl.hpp"
#include "vulkan/vulkan_handles.hpp"

#undef main
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

    unsigned int count;
    SDL_Vulkan_GetInstanceExtensions(window, &count, nullptr);
    std::vector<const char*> extensions(count);
    SDL_Vulkan_GetInstanceExtensions(window, &count, extensions.data());

    vkl::Init(
        extensions,
        [&](vk::Instance instance) {
          VkSurfaceKHR surface;
          if (!SDL_Vulkan_CreateSurface(window, instance, &surface)) {
            throw std::runtime_error("can't create surface");
          }
          return surface;
        },
        1024, 720);

    auto& renderer = vkl::GetRenderer();
    while (!shouldClose) {
        while (SDL_PollEvent(&event)) {
          if (event.type == SDL_QUIT) {
            shouldClose = true;
          }
        }
        renderer.Render();
    }

    vkl::Quit();

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}