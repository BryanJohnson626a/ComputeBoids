#include "utils.hpp"
#include "glslang/"

uint32_t FindQueueFamily(const vk::PhysicalDevice & physical_device)
{
  auto queue_family_properties = physical_device.getQueueFamilyProperties();

  for (uint32_t i = 0; i < queue_family_properties.size(); ++i)
    if (queue_family_properties[i].queueFlags & (vk::QueueFlagBits::eGraphics | vk::QueueFlagBits::eCompute))
      return i;

  return -1;
}

vk::Format GetSupportedFamily(const vk::PhysicalDevice & physical_device,
  const vk::ImageTiling & tiling, const vk::FormatFeatureFlags & features,
  const std::vector<vk::Format> & candidate_formats)
{
  for (auto & format : candidate_formats)
  {
    auto format_properties = physical_device.getFormatProperties(format);

    if (tiling == vk::ImageTiling::eLinear && (format_properties.linearTilingFeatures & features) == features)
    {
      return format;
    }
    else if (tiling == vk::ImageTiling::eOptimal && (format_properties.optimalTilingFeatures & features) == features)
    {
      return format;
    }
  }

  throw std::runtime_error("Could not get image format.");
}

uint32_t GetMemoryTypeIndex(const vk::PhysicalDevice & physical_device, const vk::MemoryPropertyFlags & flags, uint32_t memory_type_bitmask)
{
  auto memory_properties = physical_device.getMemoryProperties();

  for (uint32_t i = 0; i < memory_properties.memoryTypeCount; ++i)
    if (memory_type_bitmask & (1 << i) && (memory_properties.memoryTypes[i].propertyFlags & flags) == flags)
      return i;

  return -1;
}

void CompileShader(std::filesystem::path path)
{

}
