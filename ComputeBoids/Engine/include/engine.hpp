#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#define VULKAN_HPP_NO_CONSTRUCTORS
#define VULKAN_HPP_NO_EXCEPTIONS
#include <vulkan/vulkan.hpp>
#include <vector>


class Engine {
public:
  Engine(std::vector<const char *> required_extensions);
  ~Engine();
private:
  void init_loader();
  bool init_instance();
  bool create_debug_messenger();
  bool check_instance_extension_support();
  bool check_validation_layer_support();
  void enable_debug_validation();
  bool choose_physical_device();
  bool create_logical_device();

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

uint32_t find_queue_family(const vk::PhysicalDevice & physical_device);
