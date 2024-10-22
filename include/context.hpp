#pragma once

#include <memory>
#include <vulkan/vulkan.hpp>

namespace vkl {

class Context final {
public:
    static void Init();
    static void Quit();
    static Context& GetInstance();

    ~Context();

    vk::Instance instance;
private:
    Context();

    static std::unique_ptr<Context> instance_;
};

}