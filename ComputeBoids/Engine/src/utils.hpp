#pragma once
#include "vulkan_includes.hpp"
#include <filesystem>

uint32_t FindQueueFamily(const vk::PhysicalDevice & physical_device);

vk::Format GetSupportedFamily(const vk::PhysicalDevice & physical_device,
  const vk::ImageTiling & tiling, const vk::FormatFeatureFlags & features,
  const std::vector<vk::Format> & candidate_formats);

uint32_t GetMemoryTypeIndex(const vk::PhysicalDevice & physical_device,
  const vk::MemoryPropertyFlags & flags, uint32_t memory_type_bitmask);

void CompileShader(std::filesystem::path path);