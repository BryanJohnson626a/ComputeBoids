#include "swapchain.hpp"
#include "engine.hpp"
#include "utils.hpp"

DepthBuffer CreateDepthBuffer(const vk::PhysicalDevice & physical_device,
  vk::Device & device, const vk::Extent3D & extent)
{
  std::vector<vk::Format> formats
  {
    vk::Format::eD32Sfloat,
    vk::Format::eD32SfloatS8Uint,
    vk::Format::eD24UnormS8Uint
  };

  vk::Format format = GetSupportedFamily(physical_device, vk::ImageTiling::eOptimal,
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

  result = device.bindImageMemory(depth_buffer._image, depth_buffer._device_memory, 0);

  vk::ImageViewCreateInfo image_view_ci
  {
    .image = depth_buffer._image,
    .viewType = vk::ImageViewType::e2D,
    .format = format,
    .subresourceRange = {
      .aspectMask = vk::ImageAspectFlagBits::eDepth,
      .levelCount = 1,
      .layerCount = 1,
    },
  };

  std::tie(result, depth_buffer._image_view) = device.createImageView(image_view_ci);

  if (result != vk::Result::eSuccess)
    throw std::runtime_error("Could not create depth buffer image view.");

  return depth_buffer;
}

Swapchain::Swapchain(const vk::Instance & instance, vk::Device & device, const Surface & surface, const vk::PhysicalDevice & physicalDevice) {

  _surface_format = surface.GetPreferredImageFormat(physicalDevice);
  _extent = surface.GetSurfaceCapabilities(physicalDevice).currentExtent;

  //TODO: Handle Image Extent resizing
  //TODO: Make sure pretransform/queue family indices default values are ok

  vk::SwapchainCreateInfoKHR swapchain_ci{
      .surface = surface._handle,
      .minImageCount = surface.GetMinImageCount(physicalDevice),
      .imageFormat = _surface_format.format,
      .imageColorSpace = _surface_format.colorSpace,
      .imageExtent = _extent,
      .imageArrayLayers = 1,
      .imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
      .imageSharingMode = vk::SharingMode::eExclusive,
      .presentMode = surface.GetPreferredPresentMode(physicalDevice),
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

    if (result != vk::Result::eSuccess)
      throw std::runtime_error("Could not create swapchain image view.");
  }

  _depth_buffer = CreateDepthBuffer(physicalDevice, device, vk::Extent3D(_extent.width, _extent.height, 1));
}

bool Swapchain::AcquireNextImage(vk::Semaphore image_available_semaphore, const vk::Device & device)
{
  auto result = device.acquireNextImageKHR(_handle, 0, image_available_semaphore);

  if (result.result == vk::Result::eNotReady)
    return false;
  else if (result.result == vk::Result::eSuccess)
    return true;
  else
    throw std::runtime_error("Could not require next image.");
}

void Swapchain::PresentToQueue(vk::Queue & queue, uint32_t image_index, vk::Semaphore present_semaphore)
{
  vk::PresentInfoKHR present_i
  {
    .waitSemaphoreCount = 1,
    .pWaitSemaphores = &present_semaphore,
    .swapchainCount = 1,
    .pSwapchains = &_handle,
    .pImageIndices = &image_index,
  };

  auto result = queue.presentKHR(present_i);

  if (result != vk::Result::eSuccess)
    throw std::runtime_error("Could not present image.");
}

void Swapchain::CreateFramebuffers(const vk::Device & device, const vk::RenderPass & render_pass)
{
  vk::FramebufferCreateInfo framebuffer_ci
  {
    .renderPass = render_pass,
    .width = _extent.width,
    .height = _extent.height,
    .layers = 1,
  };

  for (auto image_view : _image_views)
  {
    std::array<vk::ImageView, 2> attachments{ image_view, _depth_buffer._image_view };

    framebuffer_ci.setAttachmentCount(attachments.size());
    framebuffer_ci.setAttachments(attachments);

    auto [result, framebuffer] = device.createFramebuffer(framebuffer_ci);

    if (result != vk::Result::eSuccess)
      throw std::runtime_error("Could not create framebuffer.");

    _framebuffers.push_back(std::move(framebuffer));
  }
}

int Swapchain::GetImageCount() const
{
  return _image_views.size();
}

vk::Extent2D Swapchain::GetExtent() const
{
  return _extent;
}
