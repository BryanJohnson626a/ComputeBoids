#pragma once
#include "vulkan_includes.hpp"
#include "surface.hpp"

struct DepthBuffer
{
  vk::Image _image;
  vk::ImageView _image_view;
  vk::DeviceMemory _device_memory;
};

class Swapchain
{
public:
    Swapchain(){};
    Swapchain(const vk::Instance& instance, const vk::Device& device, const Surface& surface, const vk::PhysicalDevice& physicalDevice);
private:
    vk::SwapchainKHR _handle;
    vk::SurfaceFormatKHR _surface_format;
    std::vector<vk::Image> _images;
    std::vector<vk::ImageView> _image_views;
    DepthBuffer _depth_buffer;
};