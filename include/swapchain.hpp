#pragma once

#include "SDL2/SDL_stdinc.h"
#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_enums.hpp"
#include "vulkan/vulkan_handles.hpp"
#include "vulkan/vulkan_structs.hpp"

namespace vkl {

class Swapchain final {
public:
    vk::SwapchainKHR swapchain;

    Swapchain(int w, int h);
    ~Swapchain();

    struct SwapchainInfo {
        vk::Extent2D imageExtent; 
        uint32_t imageCount;
        vk::SurfaceFormatKHR format;
        vk::SurfaceTransformFlagBitsKHR transform;
        vk::PresentModeKHR present;
    };

    SwapchainInfo info;
    std::vector<vk::Image> images;
    std::vector<vk::ImageView> imagesViews;

    void queryInfo(int w, int h);
    void getImages();
    void createImageViews();
};

// namespace vkl
}