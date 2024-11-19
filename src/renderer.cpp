#include "renderer.hpp"
#include "context.hpp"
#include "vulkan/vulkan_core.h"
#include "vulkan/vulkan_enums.hpp"
#include "vulkan/vulkan_handles.hpp"
#include "vulkan/vulkan_structs.hpp"
#include <array>
#include <cstdint>
#include <limits>
#include <iostream>

namespace vkl {

Renderer::Renderer() {
    initCmdPool();
    allocCmdBuf();
    createSems();
    createFence();
}

Renderer::~Renderer() {
    auto& device = Context::GetInstance().device;
    device.freeCommandBuffers(cmdPool_, cmdBuf_);
    device.destroyCommandPool(cmdPool_);
    device.destroySemaphore(imageAvailable_);
    device.destroySemaphore(imageDrawFinish_);
    device.destroyFence(cmdAvailableFence_);
}

void Renderer::Render() {
    auto& device = Context::GetInstance().device;
    auto& renderProcess = Context::GetInstance().renderProcess;
    auto& swapchain = Context::GetInstance().swapchain;

    auto result = device.acquireNextImageKHR(Context::GetInstance().swapchain->swapchain, 
                               std::numeric_limits<uint64_t>::max(), imageAvailable_);

    if(result.result != vk::Result::eSuccess) {
        std::cout << "acquire next image failed!" << std::endl;
    }

    auto imageIndex = result.value;

    cmdBuf_.reset();

    vk::CommandBufferBeginInfo begin;
    begin.setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
    cmdBuf_.begin(begin); {
        vk::RenderPassBeginInfo renderPassBegin;
        vk::Rect2D area;
        vk::ClearValue clearValue;

        clearValue.color = vk::ClearColorValue(std::array<float, 4>{0.1f, 0.1f, 0.1f, 1.0f});
        area.setOffset({0, 0})
            .setExtent(swapchain->info.imageExtent);
        renderPassBegin.setRenderPass(renderProcess->renderpass)
                       .setRenderArea(area)
                       .setFramebuffer(swapchain->framebuffers[imageIndex])
                       .setClearValues(clearValue);

        cmdBuf_.beginRenderPass(renderPassBegin, {}); {
            cmdBuf_.bindPipeline(vk::PipelineBindPoint::eGraphics, renderProcess->pipeline);
            cmdBuf_.draw(3, 1, 0, 0);
        } cmdBuf_.endRenderPass();
    } cmdBuf_.end();

    vk::SubmitInfo submit;
    vk::PipelineStageFlags waitDstStageMask[1] = {vk::PipelineStageFlags{VK_PIPELINE_STAGE_ALL_COMMANDS_BIT}};
    submit.setCommandBuffers(cmdBuf_)
          .setWaitSemaphores(imageAvailable_)
          .setSignalSemaphores(imageDrawFinish_)
          .setWaitDstStageMask(waitDstStageMask);
    Context::GetInstance().graphicsQueue.submit(submit, cmdAvailableFence_);

    vk::PresentInfoKHR present;
    present.setImageIndices(imageIndex)
           .setSwapchains(swapchain->swapchain)
           .setWaitSemaphores(imageDrawFinish_);

    if(Context::GetInstance().presentQueue.presentKHR(present) != vk::Result::eSuccess) {
        std::cout << "image present failed\n";
    }

    if(Context::GetInstance().device.waitForFences(cmdAvailableFence_, true, std::numeric_limits<uint64_t>::max())
        != vk::Result::eSuccess) {
        std::cout << "wait for fence failed\n";
    }

    Context::GetInstance().device.resetFences(cmdAvailableFence_);
}

void Renderer::initCmdPool() {
    vk::CommandPoolCreateInfo createInfo;
    createInfo.setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer);

    cmdPool_ = Context::GetInstance().device.createCommandPool(createInfo);
}

void Renderer::allocCmdBuf() {
    vk::CommandBufferAllocateInfo allocInfo;
    allocInfo.setCommandPool(cmdPool_)
             .setCommandBufferCount(1)
             .setLevel(vk::CommandBufferLevel::ePrimary);

    cmdBuf_ = Context::GetInstance().device.allocateCommandBuffers(allocInfo)[0];
}

void Renderer::createSems() {
    vk::SemaphoreCreateInfo createInfo;

    imageAvailable_ = Context::GetInstance().device.createSemaphore(createInfo);
    imageDrawFinish_ = Context::GetInstance().device.createSemaphore(createInfo);
}

void Renderer::createFence() {
    vk::FenceCreateInfo createInfo;
    cmdAvailableFence_ = Context::GetInstance().device.createFence(createInfo);
}

}