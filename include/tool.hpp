#pragma once

#include <algorithm>
#include <vector>
#include <functional>
#include "vulkan/vulkan.hpp"

using CreateSurfaceFunc = std::function<vk::SurfaceKHR(vk::Instance)>;

namespace vkl {

template<typename T, typename U>
void RemoveNoSupportedElems(std::vector<T>& elems, const std::vector<U>& supportedElems,
                            std::function<bool(const T&, const U&)> eq) {
    int i = 0;
    while(i < elems.size()) {
      if (std::find_if(supportedElems.begin(), supportedElems.end(),
                       [&](const U &e) { return eq(elems[i], e); }) 
        == supportedElems.end()) 
        {
            elems.erase(elems.begin() + i);
        }else {
            i++;
        }
    }

}

std::string ReadWholeFile(const std::string& filename);

}
