#include "renderer.hpp"

Renderer::Renderer()
{

}

Renderer::~Renderer()
{

}

void Renderer::initialize_with_surface(const vk::Instance &instance, const vk::Device &device, const Surface &surface,
                                       const vk::PhysicalDevice &physicalDevice) {
_swapchain = Swapchain(instance, device, surface, physicalDevice);
}
