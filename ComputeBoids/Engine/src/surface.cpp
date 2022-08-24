//
// Created by James on 8/24/2022.
//

#include "surface.hpp"
#include <exception>
#include <iostream>

Surface::Surface(vk::SurfaceKHR vkSurface) {
    _handle = vkSurface;
}

vk::SurfaceCapabilitiesKHR Surface::get_surface_capabilities(const vk::PhysicalDevice &physicalDevice) const {

    auto [result, value] = physicalDevice.getSurfaceCapabilitiesKHR(_handle );

    if(result != vk::Result::eSuccess)
    {
        throw std::runtime_error("Could not get surface capabilities");
    }

    return value;

}

vk::PresentModeKHR Surface::get_preferred_present_mode(const vk::PhysicalDevice &physicalDevice) const {
    auto [result, presentModes] = physicalDevice.getSurfacePresentModesKHR(_handle);

    if(result != vk::Result::eSuccess)
    {
        throw std::runtime_error("Could not get surface present modes");
    }

    if(std::find(presentModes.begin(), presentModes.end(), vk::PresentModeKHR::eMailbox) != presentModes.end())
    {
        std::cout << "Mailbox presentation supported" << std::endl;
        return vk::PresentModeKHR::eMailbox;
    }
    else if(std::find(presentModes.begin(), presentModes.end(), vk::PresentModeKHR::eFifo) != presentModes.end())
    {
        std::cout << "Mailbox presentation unsupported. Defaulting to FIFO presentation";
        return vk::PresentModeKHR::eFifo;
    }
    else
    {
        throw std::runtime_error("No presentation modes supported!");
    }
}

uint32_t Surface::get_min_image_count(const vk::PhysicalDevice &physicalDevice) const {

    vk::SurfaceCapabilitiesKHR capabilities = get_surface_capabilities(physicalDevice);

    return capabilities.minImageCount;

    //TODO: May be more cases to handle
}

vk::SurfaceFormatKHR Surface::get_preferred_image_format(const vk::PhysicalDevice& physicalDevice) const {
    auto [result, availableFormats] = physicalDevice.getSurfaceFormatsKHR(_handle);

    if(result != vk::Result::eSuccess)
    {
        throw std::runtime_error("Could not enumerate surface image formats!");
    }

    for(const auto& format : availableFormats)
    {
        if((format.format == vk::Format::eB8G8R8A8Srgb ) && (format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear))
        {
            return format;
        }
    }

    throw std::runtime_error("Couldn't find supported image format!");

    return vk::SurfaceFormatKHR();
}
