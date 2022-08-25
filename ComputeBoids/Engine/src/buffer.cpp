#include <exception>

#include "buffer.hpp"
#include "utils.hpp"

Buffer::Buffer(const vk::Instance & instance,
  const vk::Device & device,
  const vk::PhysicalDevice & physical_device,
  const vk::DeviceSize & buffer_size,
  const vk::BufferUsageFlags & usage,
  const vk::MemoryPropertyFlags & properties)
{
  vk::BufferCreateInfo buffer_ci
  {
    .size = buffer_size,
    .usage = usage,
    .sharingMode = vk::SharingMode::eExclusive,
  };

  vk::Result result;
  std::tie(result, _handle) = device.createBuffer(buffer_ci);

  if (result != vk::Result::eSuccess)
    throw std::runtime_error("Could not create buffer.");

  vk::MemoryRequirements memory_requirements = device.getBufferMemoryRequirements(_handle);

  uint32_t memory_type_index = GetMemoryTypeIndex(physical_device, properties, memory_requirements.memoryTypeBits);

  vk::MemoryAllocateInfo memory_allocate_ci
  {
    .allocationSize = memory_requirements.size,
    .memoryTypeIndex = memory_type_index,
  };

  std::tie(result, _device_memory) = device.allocateMemory(memory_allocate_ci);

  if (result != vk::Result::eSuccess)
    throw std::runtime_error("Could not allocate memory.");

  device.bindBufferMemory(_handle, _device_memory, 0);
}
