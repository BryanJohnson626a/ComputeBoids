#include "swapchain.hpp"
#include "engine.hpp"
#include "utils.hpp"

DepthBuffer create_depth_buffer(const vk::PhysicalDevice & physical_device,
  vk::Device & device, const vk::Extent3D & extent)
{
  std::vector<vk::Format> formats
  {
    vk::Format::eD32Sfloat,
    vk::Format::eD32SfloatS8Uint,
    vk::Format::eD24UnormS8Uint
  };

  vk::Format format = get_supported_format(physical_device, vk::ImageTiling::eOptimal,
    vk::FormatFeatureFlagBits::eDepthStencilAttachment, formats);

  DepthBuffer depth_buffer;

  vk::ImageCreateInfo image_ci
  {
    .imageType = vk::ImageType::e2D,
    .format = format,
    .extent = extent,
    .mipLevels = 1,
    .arrayLayers = 1,
    .samples = vk::SampleCountFlagBits::e1,
    .tiling = vk::ImageTiling::eOptimal,
    .usage = vk::ImageUsageFlagBits::eDepthStencilAttachment,
  };

  vk::Result result;
  std::tie(result, depth_buffer._image) = device.createImage(image_ci);

  if (result != vk::Result::eSuccess)
    throw std::runtime_error("Could not create depth buffer image.");

  vk::MemoryRequirements memory_requirements = device.getImageMemoryRequirements(depth_buffer._image);

  uint32_t memory_type_index = GetMemoryTypeIndex(physical_device, vk::MemoryPropertyFlagBits::eDeviceLocal, memory_requirements.memoryTypeBits);

  vk::MemoryAllocateInfo memory_allocate_info
  {
    .allocationSize = memory_requirements.size,
    .memoryTypeIndex = memory_type_index,
  };

  std::tie(result, depth_buffer._device_memory) = device.allocateMemory(memory_allocate_info);

  if (result != vk::Result::eSuccess)
    throw std::runtime_error("Could not allocate depth buffer memory.");

  device.bindImageMemory(depth_buffer._image, depth_buffer._device_memory, 0);

  // RESUME HERE

  vk::ImageViewCreateInfo image_view_ci
  {
    .viewType = vk::ImageViewType::e2D,
    .format = format,
    .subresourceRange = {
      .aspectMask = vk::ImageAspectFlagBits::eColor,
      .levelCount = 1,
      .layerCount = 1,
    },
  };

  return depth_buffer;
}

Swapchain::Swapchain(const vk::Instance & instance, const vk::Device & device, const Surface & surface, const vk::PhysicalDevice & physicalDevice) {

  _surface_format = surface.get_preferred_image_format(physicalDevice);

  //TODO: Handle Image Extent resizing
  //TODO: Make sure pretransform/queue family indices default values are ok

  vk::SwapchainCreateInfoKHR swapchain_ci{
      .surface = surface._handle,
      .minImageCount = surface.get_min_image_count(physicalDevice),
      .imageFormat = _surface_format.format,
      .imageColorSpace = _surface_format.colorSpace,
      .imageExtent = surface.get_surface_capabilities(physicalDevice).currentExtent,
      .imageArrayLayers = 1,
      .imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
      .imageSharingMode = vk::SharingMode::eExclusive,
      .presentMode = surface.get_preferred_present_mode(physicalDevice),
  };

  vk::Result result;
  std::tie(result, _handle) = device.createSwapchainKHR(swapchain_ci);

  if (result != vk::Result::eSuccess)
    throw std::runtime_error("Could not create swapchain.");

  std::tie(result, _images) = device.getSwapchainImagesKHR(_handle);

  if (result != vk::Result::eSuccess)
    throw std::runtime_error("Could not get swapchain images.");

  vk::ImageViewCreateInfo image_view_ci
  {
    .viewType = vk::ImageViewType::e2D,
    .format = _surface_format.format,
    .subresourceRange = {
      .aspectMask = vk::ImageAspectFlagBits::eColor,
      .levelCount = 1,
      .layerCount = 1,
    },
  };

  _image_views.resize(_images.size());

  for (int i = 0; i < _images.size(); ++i)
  {
    image_view_ci.image = _images[i];

    std::tie(result, _image_views[i]) = device.createImageView(image_view_ci);
  }

  auto buf = create_depth_buffer(physicalDevice, device, vk::Extent3D{});
}
