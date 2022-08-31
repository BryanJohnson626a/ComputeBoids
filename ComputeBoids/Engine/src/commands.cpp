#include "commands.hpp"

CommandDispatch::CommandDispatch(const vk::Device & device, uint32_t submit_queue_index, uint32_t num_buffers)
{
  vk::CommandPoolCreateInfo command_pool_ci
  {
    .flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
    .queueFamilyIndex = submit_queue_index,
  };

  vk::Result result;
  std::tie(result, _command_pool) = device.createCommandPool(command_pool_ci);

  vk::CommandBufferAllocateInfo command_buffer_allocate_ci
  {
    .commandPool = _command_pool,
    .level = vk::CommandBufferLevel::ePrimary,
    .commandBufferCount = num_buffers,
  };

  std::tie(result, _command_buffers) = device.allocateCommandBuffers(command_buffer_allocate_ci);
}
