#include <vulkan/vulkan.hpp>

#include "swapchain.hpp"


class Renderer
{
public:
  Renderer();
  ~Renderer();

  void initialize_with_surface(const vk::SurfaceKHR & surface);
private:

  Swapchain _swapchain;
};
