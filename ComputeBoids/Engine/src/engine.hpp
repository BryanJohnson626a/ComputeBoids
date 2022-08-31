#include <vulkan_includes.hpp>
#include <vector>
#include <GLFW/glfw3.h>
#include "renderer.hpp"

class Engine
{
public:
  Engine(std::vector<const char *> required_extensions);
  ~Engine();

  bool CreateRendererFromWindow(GLFWwindow * window_handle);
private:
  void InitializeLoader();
  bool InitializeInstance();
  bool CreateDebugMessenger();
  bool CheckInstanceExtensionSupport();
  bool CheckValidationLayerSupport();
  void EnableDebugValidation();
  bool ChoosePhysicalDevice();
  bool CreateLogicalDevice();

  Renderer _renderer;

  vk::DynamicLoader _dl;
  vk::Instance _instance;
  vk::PhysicalDevice _physical_device;
  vk::Device _device;
  vk::DebugUtilsMessengerEXT _messenger;
  uint32_t _queue_family_index;

  std::vector<const char *> _enabled_extensions;
  std::vector<const char *> _enabled_layers;

#ifndef NDEBUG
  const bool _validation_enabled = true;
#else
  const bool _validation_enabled = false;
#endif

};
