#pragma once

#include "vulkan_includes.hpp"

class CommandDispatch
{
public:
  CommandDispatch() {};
  CommandDispatch(const vk::Device & device,
    uint32_t submit_queue_index, uint32_t num_buffers);

  std::vector<vk::CommandBuffer> _command_buffers;
private:
  vk::CommandPool _command_pool;
};