#pragma once

#include "vulkan/vulkan.hpp"

namespace vkl {

class RenderProcess final {
public:
    vk::Pipeline pipeline;

    void InitPipeline(int width, int height);
    void DestroyPipeline();

};

} // namespace vkl
