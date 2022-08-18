#include "../include/engine.hpp"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

#include <iostream>

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
  VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT * pCallbackData,
  void * pUserData)
{
  std::cout << "VULKAN: " << pCallbackData->pMessage << "\n";
  return VK_FALSE;
}

Engine::Engine(std::vector<const char *> required_extensions)
{
  _enabled_extensions = required_extensions;
  init_loader();

  if (!init_instance())
    throw std::exception("Could not create instance.");


#ifndef NDEBUG
  vk::DebugUtilsMessengerCreateInfoEXT debug_create_info{
    .messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eError
                     | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning,
    .messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral
                 | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation
                 | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
    .pfnUserCallback = &debugCallback,
  };

  auto [result, value] = _instance.createDebugUtilsMessengerEXT(debug_create_info);
  _messenger = value;
#endif

  if (!choose_physical_device())
    throw std::exception("Could not find a valid physical device.");

  if (!create_logical_device())
    throw std::exception("Could not create a logical device.");
  
}

Engine::~Engine()
{
  _instance.destroyDebugUtilsMessengerEXT(_messenger);
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

#ifndef NDEBUG
  enable_debug_validation();
#endif
  if (!check_validation_layer_support())
    return false;

  if (!check_instance_extension_support())
    return false;

  vk::InstanceCreateInfo createInfo{
    .pApplicationInfo = &appInfo,
    .enabledLayerCount = (uint32_t)_enabled_layers.size(),
    .ppEnabledLayerNames = _enabled_layers.data(),
    .enabledExtensionCount = (uint32_t)_enabled_extensions.size(),
    .ppEnabledExtensionNames = _enabled_extensions.data(),
  };

  auto result = vk::createInstance(createInfo);

  if (result.result != vk::Result::eSuccess)
  {
    std::cout << "Could not create vulkan instance." << std::endl;
    return false;
  }

  std::cout << "Instance created successfully." << std::endl;

  _instance = result.value;

  VULKAN_HPP_DEFAULT_DISPATCHER.init(_instance);

  return true;
}

void Engine::init_loader()
{
  PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = _dl.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
  VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);

}

bool Engine::check_instance_extension_support()
{
  auto available_instance_extensions = vk::enumerateInstanceExtensionProperties();

  for (const auto & required_extension : _enabled_extensions)
  {
    bool found = false;
    std::cout << "Checking for " << required_extension << "... ";

    for (const auto & avalable_extension : available_instance_extensions.value)
      if (strcmp(required_extension, avalable_extension.extensionName) == 0)
      {
        std::cout << "found!\n";
        found = true;
      }

    if (!found)
    {
      std::cout << "missing!\n";
      return false;
    }
  }


  std::cout << "All required extensions are supported." << std::endl;

  return true;
}

bool Engine::check_validation_layer_support()
{
  auto available_instance_layers = vk::enumerateInstanceLayerProperties();

  for (const auto & required_layer : _enabled_layers)
  {
    bool found = false;
    std::cout << "Checking for " << required_layer << "... ";

    for (const auto & avalable_layer : available_instance_layers.value)
      if (strcmp(required_layer, avalable_layer.layerName) == 0)
      {
        std::cout << "found!\n";
        found = true;
      }

    if (!found)
    {
      std::cout << "missing!\n";
      return false;
    }
  }


  std::cout << "All required layers are supported." << std::endl;

  return true;
}

void Engine::enable_debug_validation()
{
  _enabled_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  _enabled_layers.push_back("VK_LAYER_KHRONOS_validation");
}

bool is_physical_device_valid(vk::PhysicalDevice device)
{
  auto properties = device.getProperties();
  auto features = device.getFeatures();

  if (properties.deviceType != vk::PhysicalDeviceType::eDiscreteGpu)
    return false;

  return true;
}

bool Engine::choose_physical_device()
{
  auto [result, physical_devices] = _instance.enumeratePhysicalDevices();

  if (result != vk::Result::eSuccess)
  {
    std::cout << "Failed.";
    return false;
  }

  for (auto & physical_device : physical_devices)
    if (is_physical_device_valid(physical_device))
    {
      _device = physical_device;
      return true;
    }
  
  return false;
}

bool Engine::create_logical_device()
{

  return true;
}
