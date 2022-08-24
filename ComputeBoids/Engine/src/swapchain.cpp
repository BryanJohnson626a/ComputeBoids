#include "swapchain.hpp"

Swapchain::Swapchain(const vk::Instance &instance, const vk::Device &device, const Surface &surface, const vk::PhysicalDevice& physicalDevice) {

    _surface_format = surface.get_preferred_image_format(physicalDevice);

    //TODO: Handle Image Extent
    //TODO: Make sure pretransform/queue family indices default values are ok

    vk::SwapchainCreateInfoKHR swapchainCF{
        .surface = surface._handle,
        .minImageCount = surface.get_min_image_count(physicalDevice),
        .imageFormat = _surface_format.format,
        .imageColorSpace = _surface_format.colorSpace,
        .imageExtent = vk::Extent2D(),
        .imageArrayLayers = 1,
        .imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
        .imageSharingMode = vk::SharingMode::eExclusive,
        .presentMode = surface.get_preferred_present_mode(physicalDevice),
    };

    auto[result, swapchain] = device.createSwapchainKHR(swapchainCF);

    if(result != vk::Result::eSuccess)
    {
        throw std::runtime_error("Could not create Swapchain");
    }

    _handle = swapchain;

}
