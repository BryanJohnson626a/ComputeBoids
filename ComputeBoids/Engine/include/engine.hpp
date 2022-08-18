
#define VULKAN_HPP_NO_CONSTRUCTORS
#define VULKAN_HPP_NO_EXCEPTIONS
#include "vulkan/vulkan.hpp"

class Engine {
public:
  Engine();
private:
  bool init_instance();
  bool check_instance_extension_support(const std::vector<vk::ExtensionProperties>& required_extensions);
  vk::Instance _instance;

};