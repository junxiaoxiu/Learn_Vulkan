#include "vkl.hpp"
#include "context.hpp"
#include "shader.hpp"
#include "tool.hpp"

namespace vkl {

void Init(const std::vector<const char*>& extensions, CreateSurfaceFunc func, int w, int h) {
    Context::Init(extensions, func);
    Context::GetInstance().InitSwapChain(w, h);
    Shader::Init(ReadWholeFile("./vert.spv"), ReadWholeFile("./frag.spv"));
    Context::GetInstance().renderProcess->InitRenderPass();
    Context::GetInstance().renderProcess->InitLayout();
    Context::GetInstance().swapchain->createFramebuffers(w, h);
    Context::GetInstance().renderProcess->InitPipeline(w, h);
    Context::GetInstance().InitRenderer();
}

void Quit() { 
    Context::GetInstance().device.waitIdle();
    Context::GetInstance().DestroyRenderer();
    Context::GetInstance().renderProcess.reset();
    Context::GetInstance().DestroySwapchain();
    Shader::Quit();
    Context::Quit();
}

Renderer& GetRenderer() {
    return *Context::GetInstance().renderer;
}

} // namespace vkl