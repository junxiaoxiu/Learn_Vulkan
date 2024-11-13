#pragma once

#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_structs.hpp"
#include <memory>
#include <string>

namespace vkl {

class Shader final {
public:
    static void Init(const std::string& vertexSource, const std::string& fragSource);
    static void Quit();

    static Shader& GetInstance();

    vk::ShaderModule vertexModule;
    vk::ShaderModule fragModule;

    ~Shader();

    std::vector<vk::PipelineShaderStageCreateInfo> GetStage();

private:
    static std::unique_ptr<Shader> instance_;
    std::vector<vk::PipelineShaderStageCreateInfo> stage_;

    Shader(const std::string& vertexSource, const std::string& fragSource);
    void initStage();
};

} // namespace vkl