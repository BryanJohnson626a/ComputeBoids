#pragma once

#include "swapchain.hpp"
#include "commands.hpp"

class Renderer
{
public:
  void InitializeWithSurface(const vk::Instance & instance,
    vk::Device & device, const Surface & surface,
    const vk::PhysicalDevice & physicalDevice,
    uint32_t submit_queue_index);

  void RecordTestCommand();
private:
  void InitializeCommandDispatch(const vk::Device & device, uint32_t submit_queue_index);

  CommandDispatch _dispatch;
  Swapchain _swapchain;
};
