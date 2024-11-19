#pragma once 
 
#include <vulkan/vulkan.hpp>
#include "context.hpp"
#include "renderer.hpp"
#include "tool.hpp"
#include "shader.hpp"

namespace vkl {


void Init(const std::vector<const char*>& extensions, CreateSurfaceFunc func, int w, int h);
void Quit();

Renderer& GetRenderer();

}