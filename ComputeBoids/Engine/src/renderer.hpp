#include "swapchain.hpp"


class Renderer
{
public:
  Renderer();
  ~Renderer();

  void initialize_with_surface(const vk::Instance& instance, const vk::Device& device, const Surface& surface, const vk::PhysicalDevice& physicalDevice);
private:
    Swapchain _swapchain;
};
