#pragma once

#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_handles.hpp"

namespace vkl {

class RenderProcess final {
public:
    vk::Pipeline pipeline;
    vk::PipelineLayout layout;
    vk::RenderPass renderpass;

    ~RenderProcess();

    void InitLayout();
    void InitRenderPass();
    void InitPipeline(int width, int height);
};

} // namespace vkl
