#include "context.hpp"
#include "SDL2/SDL_gamecontroller.h"
#include "render_process.hpp"
#include "swapchain.hpp"
#include "vulkan/vulkan.hpp"
#include <cassert>
#include "tool.hpp"

#include <iostream>

namespace vkl {

std::unique_ptr<Context> Context::instance_ = nullptr;

void Context::Init(const std::vector<const char*>& extensions, CreateSurfaceFunc func){
    instance_.reset(new Context(extensions, func));
}

void Context::Quit() {
    instance_.reset();
}

Context::Context(const std::vector<const char*>& extensions, CreateSurfaceFunc func) {
    createInstance(extensions);    
    pickUpPhysicalDevice();
    surface = func(instance);
    queryQueueFamilyIndices();
    createDevice();
    getQueues();
    renderProcess.reset(new RenderProcess);
}

Context::~Context() {
    instance.destroySurfaceKHR(surface);
    device.destroy();
    instance.destroy();
}

void Context::createInstance(const std::vector<const char*>& extensions) {
    vk::InstanceCreateInfo createInfo;
    vk::ApplicationInfo appInfo;

    appInfo.setApiVersion(VK_API_VERSION_1_3);
    createInfo.setPApplicationInfo(&appInfo);
    instance = vk::createInstance(createInfo);

    std::vector<const char*> layers = {"VK_LAYER_KHRONOS_validation"};

    RemoveNoSupportedElems<const char*, vk::LayerProperties>(layers, vk::enumerateInstanceLayerProperties(),
                                    [](const char* e1, const vk::LayerProperties& e2) {
                                        return std::strcmp(e1, e2.layerName) == 0;
                                    });

    createInfo.setPEnabledLayerNames(layers)
              .setPEnabledExtensionNames(extensions);

    instance = vk::createInstance(createInfo);
}

void Context::pickUpPhysicalDevice() {
    auto devices = instance.enumeratePhysicalDevices();
    phyDevice = devices[0];

    std::cout << "picked divice = " << phyDevice.getProperties().deviceName << '\n';
}

void Context::createDevice() {
    std::array extensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME}; 

    vk::DeviceCreateInfo createInfo;
    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
    float priorities = 1.0;

    if(queueFamilyIndices.presentQueue.value() == queueFamilyIndices.graphicsQueue.value()) {
        vk::DeviceQueueCreateInfo queueCreateInfo;
        queueCreateInfo.setPQueuePriorities(&priorities)
                       .setQueueCount(1)
                       .setQueueFamilyIndex(queueFamilyIndices.graphicsQueue.value());
        queueCreateInfos.push_back(std::move(queueCreateInfo));
    } else {
        vk::DeviceQueueCreateInfo queueCreateInfo;
        queueCreateInfo.setPQueuePriorities(&priorities)
                       .setQueueCount(1)
                       .setQueueFamilyIndex(queueFamilyIndices.graphicsQueue.value());
        queueCreateInfos.push_back(queueCreateInfo);
        queueCreateInfo.setPQueuePriorities(&priorities)
                       .setQueueCount(1)
                       .setQueueFamilyIndex(queueFamilyIndices.presentQueue.value());
        queueCreateInfos.push_back(queueCreateInfo);
    }

    createInfo.setQueueCreateInfos(queueCreateInfos)
              .setPEnabledExtensionNames(extensions);
    
    device = phyDevice.createDevice(createInfo);
}

void Context::queryQueueFamilyIndices() {
    auto properties = phyDevice.getQueueFamilyProperties();
    for(int i = 0; i < properties.size(); i++) {
        const auto& property = properties[i];
        if(property.queueFlags & vk::QueueFlagBits::eGraphics) {
            queueFamilyIndices.graphicsQueue = i;
        }
        if(phyDevice.getSurfaceSupportKHR(i, surface)) {
            queueFamilyIndices.presentQueue = i;
        }
        if(queueFamilyIndices) {
            break;
        }
    }
}

void Context::getQueues() {
    graphicsQueue = device.getQueue(queueFamilyIndices.graphicsQueue.value(), 0);
    presentQueue  = device.getQueue(queueFamilyIndices.presentQueue.value(), 0);
}
// namespace vkl
}