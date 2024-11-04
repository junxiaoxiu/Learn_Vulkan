#include "vkl.hpp"
#include "context.hpp"

namespace vkl {

void Init(const std::vector<const char*>& extensions, CreateSurfaceFunc func, int w, int h) {
    Context::Init(extensions, func);
    Context::GetInstance().InitSwapChain(w, h);
}
void Quit() { 
    Context::GetInstance().DestroySwapchain();
    Context::Quit();
}

} // namespace vkl