#include "../include/engine.hpp"

#include <iostream>

Engine::Engine()
{
  if (!init_instance())
  {

  }
}

bool Engine::init_instance()
{
  vk::ApplicationInfo appInfo{
      .pApplicationName = "test",
      .applicationVersion = 1,
      .pEngineName = "test",
      .engineVersion = 1,
      .apiVersion = VK_API_VERSION_1_3,
  };

  vk::InstanceCreateInfo createInfo{ .pApplicationInfo = &appInfo };

  vk::ResultValue<vk::Instance> result = vk::createInstance(createInfo);

  if (result.result != vk::Result::eSuccess)
  {
    std::cout << "could not create vulkan instance" << std::endl;
    return false;
  }

  std::cout << "instance created successfully" << std::endl;

  _instance = result.value;

  return true;

}

bool Engine::check_instance_extension_support(const std::vector<vk::ExtensionProperties> & required_extensions)
{
  vk::ResultValue<std::vector<vk::ExtensionProperties>> available_instance_extensions = vk::enumerateInstanceExtensionProperties();

  for (auto required_extension : required_extensions)
  {

  }

  return true;
}