#pragma once

#include "SDL2/SDL_stdinc.h"
#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_handles.hpp"
#include <memory>
#include <cassert>
#include <iostream>
#include <optional>

namespace vkl {

class Context final {
public:
    static void Init();
    static void Quit();
    static Context& GetInstance();

    ~Context();

    struct QueueFamilyIndices final {
        std::optional<uint32_t> graphicsQueue;
    };

    vk::Instance instance;
    vk::PhysicalDevice phyDevice;
    vk::Device device;
    vk::Queue graphicsQueue;
    QueueFamilyIndices queueFamilyIndices;

private:
    Context();

    static std::unique_ptr<Context> instance_;

    void createInstance();
    void pickUpPhysicalDevice();
    void createDevice();
    void getQueues();

    void queryQueueFamilyIndices();

};

}