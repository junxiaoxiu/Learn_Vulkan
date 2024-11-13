#include "vkl.hpp"
#include "context.hpp"
#include "shader.hpp"
#include "tool.hpp"

namespace vkl {

void Init(const std::vector<const char*>& extensions, CreateSurfaceFunc func, int w, int h) {
    Context::Init(extensions, func);
    Context::GetInstance().InitSwapChain(w, h);
    Shader::Init(ReadWholeFile("./vert.spv"), ReadWholeFile("./frag.spv"));
    Context::GetInstance().renderProcess->InitPipeline(w, h);
}
void Quit() { 
    Context::GetInstance().renderProcess->DestroyPipeline();
    Context::GetInstance().DestroySwapchain();
    Shader::Quit();
    Context::Quit();
}

} // namespace vkl