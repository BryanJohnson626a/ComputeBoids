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
  bool init_instance();
  void init_loader();
  bool check_instance_extension_support();
  bool check_validation_layer_support();
  void enable_debug_validation();
  bool choose_physical_device();
  bool create_logical_device();

  vk::DynamicLoader _dl;
  vk::Instance _instance;
  vk::PhysicalDevice _device;
  vk::DebugUtilsMessengerEXT _messenger;

  std::vector<const char *> _enabled_extensions;
  std::vector<const char *> _enabled_layers;
};
