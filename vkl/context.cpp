#include "context.hpp"
#include "vulkan/vulkan.hpp"
#include <cassert>

namespace vkl {

std::unique_ptr<Context> Context::instance_ = nullptr;

void Context::Init(){
    instance_.reset(new Context);
}

void Context::Quit() {
    instance_.reset();
}

Context& Context::GetInstance() {
    assert(_instance);
    return *instance_;
}

Context::Context() {
    createInstance();    
    pickUpPhysicalDevice();
    queryQueueFamilyIndices();
    createDevice();
    getQueues();
}

Context::~Context() {
    device.destroy();
    instance.destroy();
}

void Context::createInstance() {
    vk::InstanceCreateInfo createInfo;
    vk::ApplicationInfo appInfo;

    appInfo.setApiVersion(VK_VERSION_1_3);
    createInfo.setPApplicationInfo(&appInfo);
    instance = vk::createInstance(createInfo);

    std::vector<const char*> layers = {"VK_LAYER_KHRONOS_validation"};

    createInfo.setPEnabledLayerNames(layers);

    instance = vk::createInstance(createInfo);
}

void Context::pickUpPhysicalDevice() {
    auto devices = instance.enumeratePhysicalDevices();
    phyDevice = devices[0];

    std::cout << phyDevice.getProperties().deviceName << std::endl;

}

void Context::createDevice() {
    vk::DeviceCreateInfo createInfo;
    vk::DeviceQueueCreateInfo queueCreateInfo;
    float priorities = 1.0;

    queueCreateInfo.setPQueuePriorities(&priorities)
                   .setQueueCount(1)
                   .setQueueFamilyIndex(queueFamilyIndices.graphicsQueue.value());
    createInfo.setQueueCreateInfos(queueCreateInfo);
    
    device = phyDevice.createDevice(createInfo);
}

void Context::queryQueueFamilyIndices() {
    auto properties = phyDevice.getQueueFamilyProperties();
    for(int i = 0; i < properties.size(); i++) {
        const auto& property = properties[i];
        if(property.queueFlags & vk::QueueFlagBits::eGraphics) {
            queueFamilyIndices.graphicsQueue = i;
        }
    }
}

void Context::getQueues() {
    device.getQueue(queueFamilyIndices.graphicsQueue.value(), 0);
}
// namespace vkl
}