#include "vulkan_includes.hpp"
#include "surface.hpp"

class Swapchain
{
public:
    Swapchain(){};
    Swapchain(const vk::Instance& instance, const vk::Device& device, const Surface& surface, const vk::PhysicalDevice& physicalDevice);
private:
    vk::SwapchainKHR _handle;
    vk::SurfaceFormatKHR _surface_format;
};