#pragma once

#include "vulkan/vulkan.hpp"

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
    std::vector<vk::Framebuffer> framebuffers;

    void queryInfo(int w, int h);
    void getImages();
    void createImageViews();
    void createFramebuffers(int w, int h);
};

// namespace vkl
}