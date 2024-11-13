#include "render_process.hpp"
#include "shader.hpp"
#include "context.hpp"
#include "vulkan/vulkan_structs.hpp"
#include <stdexcept>

namespace vkl {

void RenderProcess::InitPipeline(int width, int height) {
    vk::GraphicsPipelineCreateInfo createInfo;

    // Vertex Input
    vk::PipelineVertexInputStateCreateInfo inputState;
    createInfo.setPVertexInputState(&inputState);

    // Vertex Assembly
    vk::PipelineInputAssemblyStateCreateInfo inputAsm;
    inputAsm.setPrimitiveRestartEnable(false)
            .setTopology(vk::PrimitiveTopology::eTriangleList);
    createInfo.setPInputAssemblyState(&inputAsm);

    // Shader
    auto stages = Shader::GetInstance().GetStage();
    createInfo.setStages(stages);

    // Viewport
    vk::PipelineViewportStateCreateInfo viewportState;
    vk::Viewport viewport(0, 0, width, height, 0, 1);
    viewportState.setViewports(viewport);
    vk::Rect2D rect({0,0}, {static_cast<uint32_t>(width), static_cast<uint32_t>(height)});
    viewportState.setScissors(rect);
    createInfo.setPViewportState(&viewportState);

    // Rasterization
    vk::PipelineRasterizationStateCreateInfo rastInfo;
    rastInfo.setRasterizerDiscardEnable(false)
            .setCullMode(vk::CullModeFlagBits::eBack)
            .setFrontFace(vk::FrontFace::eCounterClockwise)
            .setPolygonMode(vk::PolygonMode::eFill)
            .setLineWidth(1);
    createInfo.setPRasterizationState(&rastInfo);

    // MultiSample
    vk::PipelineMultisampleStateCreateInfo multisampleInfo;
    multisampleInfo.setSampleShadingEnable(false)
                   .setRasterizationSamples(vk::SampleCountFlagBits::e1);
    createInfo.setPMultisampleState(&multisampleInfo);

    // test - strencil / depth

    // ColorBlending
    vk::PipelineColorBlendStateCreateInfo blendInfo;
    vk::PipelineColorBlendAttachmentState attachs;
    attachs.setBlendEnable(false)
           .setColorWriteMask(vk::ColorComponentFlagBits::eA |
                              vk::ColorComponentFlagBits::eB |
                              vk::ColorComponentFlagBits::eG |
                              vk::ColorComponentFlagBits::eR );
    
    blendInfo.setLogicOpEnable(false)
             .setAttachments(attachs);
    createInfo.setPColorBlendState(&blendInfo);

    auto result = Context::GetInstance().device.createGraphicsPipeline(nullptr, createInfo);
    if(result.result != vk::Result::eSuccess) {
        throw std::runtime_error("create graphics pipeline failed");
    }
    pipeline = result.value;
}

void RenderProcess::DestroyPipeline() {
    Context::GetInstance().device.destroyPipeline(pipeline);
}

} // namespace vkl
