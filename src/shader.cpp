#include "shader.hpp"
#include <memory>
#include "context.hpp"
#include "vulkan/vulkan_enums.hpp"
#include "vulkan/vulkan_structs.hpp"

namespace vkl {

std::unique_ptr<Shader> Shader::instance_ = nullptr;

void Shader::Init(const std::string &vertexSource,const std::string &fragSource) {
    instance_.reset(new Shader(vertexSource, fragSource));
}
void Shader::Quit() {
    instance_.reset();
}

Shader& Shader::GetInstance() {
    return *instance_;
}

Shader::Shader(const std::string &vertexSource, const std::string &fragSource) {
    vk::ShaderModuleCreateInfo createInfo;
    createInfo.codeSize = vertexSource.size();
    createInfo.pCode = (uint32_t*)vertexSource.data();
    vertexModule = Context::GetInstance().device.createShaderModule(createInfo);

    createInfo.codeSize = fragSource.size();
    createInfo.pCode = (uint32_t*)fragSource.data();
    fragModule = Context::GetInstance().device.createShaderModule(createInfo);
}

Shader::~Shader() {
    auto& device = Context::GetInstance().device;
    device.destroyShaderModule(vertexModule);
    device.destroyShaderModule(fragModule);
}

void Shader::initStage() {
    stage_.resize(2);
    stage_[0].setStage(vk::ShaderStageFlagBits::eVertex)
                  .setModule(Shader::GetInstance().vertexModule)
                  .setPName("main");

    stage_[1].setStage(vk::ShaderStageFlagBits::eFragment)
                  .setModule(Shader::GetInstance().fragModule)
                  .setPName("main");
}

std::vector<vk::PipelineShaderStageCreateInfo> Shader::GetStage() {
    return stage_;
}

} // namespace vkl