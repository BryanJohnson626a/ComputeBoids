#pragma once
#include "vulkan_includes.hpp"

uint32_t find_queue_family(const vk::PhysicalDevice & physical_device);

vk::Format get_supported_format(const vk::PhysicalDevice & physical_device,
  const vk::ImageTiling & tiling, const vk::FormatFeatureFlags & features,
  const std::vector<vk::Format> & candidate_formats);

uint32_t GetMemoryTypeIndex(const vk::PhysicalDevice & physical_device,
  const vk::MemoryPropertyFlags & flags, uint32_t memory_type_bitmask);
