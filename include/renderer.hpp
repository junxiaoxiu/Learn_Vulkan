#pragma once

#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_handles.hpp"

namespace vkl {

class Renderer final {
public:
    void Init();

    Renderer();
    ~Renderer();

    void Render();

private:
    vk::CommandPool cmdPool_;
    vk::CommandBuffer cmdBuf_;

    vk::Semaphore imageAvailable_;
    vk::Semaphore imageDrawFinish_;
    vk::Fence cmdAvailableFence_;

    void initCmdPool();
    void allocCmdBuf();
    void createSems();
    void createFence();
};

}