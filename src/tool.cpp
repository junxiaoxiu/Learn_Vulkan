#include "tool.hpp"
#include "vulkan/vulkan.hpp"
#include <fstream>
#include <iostream>

namespace vkl {

std::string ReadWholeFile(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if(!file.is_open()) {
        std::cout << "read " << filename << " failed" << std::endl;
        return std::string{};
    }

    auto size = file.tellg();
    std::string content;
    content.resize(size);
    file.seekg(0);

    file.read(content.data(), content.size());

    return content;
}

} // namespace vkl