#include <vulkan_includes.hpp>
#include <vector>
#include <GLFW/glfw3.h>
#include "renderer.hpp"

class Engine
{
public:
  Engine(std::vector<const char *> required_extensions);
  ~Engine();

  bool create_renderer_from_window(GLFWwindow * window_handle);
private:
  void init_loader();
  bool init_instance();
  bool create_debug_messenger();
  bool check_instance_extension_support();
  bool check_validation_layer_support();
  void enable_debug_validation();
  bool choose_physical_device();
  bool create_logical_device();

  Renderer _renderer;

  vk::DynamicLoader _dl;
  vk::Instance _instance;
  vk::PhysicalDevice _physical_device;
  vk::Device _device;
  vk::DebugUtilsMessengerEXT _messenger;

  std::vector<const char *> _enabled_extensions;
  std::vector<const char *> _enabled_layers;

#ifndef NDEBUG
  const bool _validation_enabled = true;
#else
  const bool _validation_enabled = false;
#endif

};
