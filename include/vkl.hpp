#pragma once 
 
#include <vulkan/vulkan.hpp>
#include "tool.hpp"

namespace vkl {


void Init(const std::vector<const char*>& extensions, CreateSurfaceFunc func, int w, int h);
void Quit();

}