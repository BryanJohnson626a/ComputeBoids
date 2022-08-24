//
// Created by James on 8/24/2022.
//
#include <vulkan_includes.hpp>
#pragma once
class Surface
{
public:
    Surface(vk::SurfaceKHR vkSurface);
    [[nodiscard]] vk::SurfaceCapabilitiesKHR get_surface_capabilities(const vk::PhysicalDevice &physicalDevice) const;
    [[nodiscard]] vk::PresentModeKHR get_preferred_present_mode(const vk::PhysicalDevice& physicalDevice) const;
    [[nodiscard]] vk::SurfaceFormatKHR get_preferred_image_format(const vk::PhysicalDevice& physicalDevice) const;
    [[nodiscard]] uint32_t get_min_image_count(const vk::PhysicalDevice& physicalDevice) const;
    vk::SurfaceKHR _handle;
private:


};
