#include "renderer.hpp"


void Renderer::InitializeWithSurface(const vk::Instance & instance,
  vk::Device & device, const Surface & surface,
  const vk::PhysicalDevice & physicalDevice,
  uint32_t submit_queue_index)
{
  _swapchain = Swapchain(instance, device, surface, physicalDevice);

  InitializeCommandDispatch(device, submit_queue_index);
}

void Renderer::RecordTestCommand()
{
  for (int i = 0; i < _swapchain.GetImageCount(); ++i)
  {
    vk::RenderingAttachmentInfo color_attach_i
    {
      .imageView = _swapchain._image_views[i],
      .imageLayout = vk::ImageLayout::eColorAttachmentOptimal,
      .loadOp = vk::AttachmentLoadOp::eClear,
      .storeOp = vk::AttachmentStoreOp::eStore,
    };

    vk::RenderingInfo rendering_i
    {
      .renderArea = vk::Rect2D({0,0}, _swapchain.GetExtent()),
      .layerCount = 1,
      .colorAttachmentCount = 1,
      .pColorAttachments = &color_attach_i,
    };

    auto cb = _dispatch._command_buffers[i];

    cb.beginRendering(rendering_i);
    cb.draw(3, 1, 0, 0);
    //cb.draw(3, 1, 1, 0);
    cb.endRendering();
  }
}

void Renderer::InitializeCommandDispatch(const vk::Device & device, uint32_t submit_queue_index)
{
  int image_count = _swapchain.GetImageCount();

  _dispatch = CommandDispatch(device, submit_queue_index, image_count);
}

