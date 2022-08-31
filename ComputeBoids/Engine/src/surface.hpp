#pragma once
#include <vulkan_includes.hpp>
class Surface
{
public:
    Surface(vk::SurfaceKHR vkSurface);
    [[nodiscard]] vk::SurfaceCapabilitiesKHR GetSurfaceCapabilities(const vk::PhysicalDevice &physicalDevice) const;
    [[nodiscard]] vk::PresentModeKHR GetPreferredPresentMode(const vk::PhysicalDevice& physicalDevice) const;
    [[nodiscard]] vk::SurfaceFormatKHR GetPreferredImageFormat(const vk::PhysicalDevice& physicalDevice) const;
    [[nodiscard]] uint32_t GetMinImageCount(const vk::PhysicalDevice& physicalDevice) const;
    vk::SurfaceKHR _handle;
private:


};
