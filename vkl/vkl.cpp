#include "vkl.hpp"
#include "context.hpp"

namespace vkl {

void Init() {
    Context::Init();
}
void Quit() {
    Context::Quit();
}

} // namespace vkl