#pragma once

#include "vulkan_includes.hpp"

class Buffer
{
public:
  Buffer(const vk::Instance & instance,
    const vk::Device & device,
    const vk::PhysicalDevice & physical_device,
    const vk::DeviceSize & buffer_size,
    const vk::BufferUsageFlags & usage,
    const vk::MemoryPropertyFlags & properties);

  vk::Buffer _handle;
private:
  vk::DeviceMemory _device_memory;
  void * _mapped_memory;
};