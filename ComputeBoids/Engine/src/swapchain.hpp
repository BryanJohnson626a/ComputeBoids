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
  Swapchain() {};
  Swapchain(const vk::Instance & instance, vk::Device & device,
    const Surface & surface, const vk::PhysicalDevice & physicalDevice);

  bool AcquireNextImage(vk::Semaphore image_available_semaphore, const vk::Device & device);
  void PresentToQueue(vk::Queue & queue, uint32_t image_index, vk::Semaphore present_semaphore);
  void CreateFramebuffers(const vk::Device & device, const vk::RenderPass & render_pass);

  int GetImageCount() const;
  vk::Extent2D GetExtent() const;

  std::vector<vk::ImageView> _image_views;
private:
  vk::SwapchainKHR _handle;
  vk::SurfaceFormatKHR _surface_format;
  std::vector<vk::Image> _images;
  DepthBuffer _depth_buffer;
  std::vector<vk::Framebuffer> _framebuffers;
  vk::Extent2D _extent;
};