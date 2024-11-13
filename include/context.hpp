#pragma once

#include "SDL2/SDL_stdinc.h"
#include "swapchain.hpp"
#include "tool.hpp"
#include "vkl.hpp"
#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_handles.hpp"
#include <cstdint>
#include <memory>
#include <cassert>
#include <optional>
#include "render_process.hpp"

namespace vkl {

class Context final {
public:
    static void Init(const std::vector<const char*>& extensions, CreateSurfaceFunc func);
    static void Quit();
    static Context& GetInstance(){
        assert(instance_);
        return *instance_;
    }

    ~Context();

    struct QueueFamilyIndices final {
        std::optional<uint32_t> graphicsQueue;
        std::optional<uint32_t> presentQueue;

        operator bool() {
            return graphicsQueue.has_value() && presentQueue.has_value();
        }
    };

    vk::Instance instance;
    vk::PhysicalDevice phyDevice;
    vk::Device device;
    vk::Queue graphicsQueue;
    vk::Queue presentQueue;
    vk::SurfaceKHR surface;
    std::unique_ptr<Swapchain> swapchain;
    std::unique_ptr<RenderProcess> renderProcess;
    QueueFamilyIndices queueFamilyIndices;

    void InitSwapChain(int w, int h) {
        swapchain.reset(new Swapchain(w, h));
    }

    void DestroySwapchain(){
        swapchain.reset();
    }
private:
    Context(const std::vector<const char*>& extensions, CreateSurfaceFunc func);

    static std::unique_ptr<Context> instance_;

    void createInstance(const std::vector<const char*>& extensions);
    void pickUpPhysicalDevice();
    void createDevice();
    void getQueues();

    void queryQueueFamilyIndices();

};

}