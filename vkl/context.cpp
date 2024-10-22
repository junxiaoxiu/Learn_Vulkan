#include "context.hpp"
#include "vulkan/vulkan_structs.hpp"

namespace vkl {

std::unique_ptr<Context> Context::instance_ = nullptr;

void Context::Init(){
    instance_.reset(new Context);
}

void Context::Quit() {
    instance_.reset();
}

Context& Context::GetInstance() {
    return *instance_;
}

Context::Context() {
    vk::InstanceCreateInfo createInfo;

    std::vector<const char*> layers = {"VK_LAYER_KHRONOS_validation"};

    vk::ApplicationInfo appInfo;
    appInfo.setApiVersion(VK_API_VERSION_1_3);

    createInfo.setPApplicationInfo(&appInfo)
              .setPEnabledLayerNames(layers);

    instance = vk::createInstance(createInfo);
}

Context::~Context() {
    instance.destroy();
}

}