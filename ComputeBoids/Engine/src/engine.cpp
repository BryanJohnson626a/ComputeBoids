#include <iostream>
#include <exception>

#include "engine.hpp"
#include "surface.hpp"
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
  VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT * pCallbackData,
  void * pUserData)
{
  std::cout << "VULKAN: " << pCallbackData->pMessage << "\n";
  return VK_FALSE;
}

Engine::Engine(std::vector<const char *> required_extensions) :
  _enabled_extensions(required_extensions)
{
  init_loader();

  if (!init_instance())
    throw std::runtime_error("Could not create instance.");

#ifndef NDEBUG
  if (!create_debug_messenger())
    throw std::runtime_error("Could not create debug messenger.");
#endif

  if (!choose_physical_device())
    throw std::runtime_error("Could not find a valid physical device.");

  if (!create_logical_device())
    throw std::runtime_error("Could not create a logical device.");

}

Engine::~Engine()
{
  _instance.destroyDebugUtilsMessengerEXT(_messenger);
}

void Engine::init_loader()
{
  PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = _dl.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
  VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);

}

bool Engine::init_instance()
{
  vk::ApplicationInfo application_info{
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

  vk::InstanceCreateInfo instance_ci{
    .pApplicationInfo = &application_info,
    .enabledLayerCount = (uint32_t)_enabled_layers.size(),
    .ppEnabledLayerNames = _enabled_layers.data(),
    .enabledExtensionCount = (uint32_t)_enabled_extensions.size(),
    .ppEnabledExtensionNames = _enabled_extensions.data(),
  };

  auto result = vk::createInstance(instance_ci);

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

bool Engine::create_debug_messenger()
{
  vk::DebugUtilsMessengerCreateInfoEXT debug_ci{
    .messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eError
                     | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning,
    .messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral
                 | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation
                 | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
    .pfnUserCallback = &debugCallback,
  };

  auto [result, value] = _instance.createDebugUtilsMessengerEXT(debug_ci);

  if (result != vk::Result::eSuccess)
    return false;

  _messenger = value;

  return true;
}

bool Engine::check_instance_extension_support()
{
  auto available_instance_extensions = vk::enumerateInstanceExtensionProperties();

  for (const auto & required_extension : _enabled_extensions)
  {
    bool found = false;
    std::cout << "Checking for " << required_extension << "... ";

    for (const auto & available_extension : available_instance_extensions.value)
      if (strcmp(required_extension, available_extension.extensionName) == 0)
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

    for (const auto & available_layer : available_instance_layers.value)
      if (strcmp(required_layer, available_layer.layerName) == 0)
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
      _physical_device = physical_device;
      return true;
    }

  return false;
}

uint32_t find_queue_family(const vk::PhysicalDevice & physical_device)
{
  auto queue_family_properties = physical_device.getQueueFamilyProperties();

  for (uint32_t i = 0; i < queue_family_properties.size(); ++i)
    if (queue_family_properties[i].queueFlags & (vk::QueueFlagBits::eGraphics | vk::QueueFlagBits::eCompute))
      return i;

  return -1;
}

bool Engine::create_logical_device()
{
  auto queue_family_index = find_queue_family(_physical_device);

  //TODO: Handle device extensions better
  std::vector<const char *> extensions;
  extensions.push_back("VK_KHR_swapchain");

  float queue_priority[1] = { 1 };
  vk::DeviceQueueCreateInfo device_queue_ci
  {
    .queueFamilyIndex = queue_family_index,
    .queueCount = 1,
    .pQueuePriorities = queue_priority,
  };

  vk::DeviceCreateInfo device_ci
  {
    .queueCreateInfoCount = 1,
    .pQueueCreateInfos = &device_queue_ci,
    .enabledExtensionCount = (uint32_t)extensions.size(),
    .ppEnabledExtensionNames = extensions.data()
  };
[]
  if (_validation_enabled)
  {
    device_ci.setEnabledLayerCount((uint32_t)_enabled_layers.size());
    device_ci.setPEnabledLayerNames(_enabled_layers);
  }

  auto [result, value] = _physical_device.createDevice(device_ci);

  if (result != vk::Result::eSuccess)
    return false;

  std::cout << "Successfully created logical device.\n";

  _device = value;

  return true;
}

bool Engine::create_renderer_from_window(GLFWwindow * window_handle)
{
  VkSurfaceKHR surface_cstyle;
  auto result = glfwCreateWindowSurface(_instance, window_handle, nullptr, &surface_cstyle);

  Surface surface(surface_cstyle);

  //vk::SurfaceKHR surface = surface_cstyle;

  if (result != VkResult::VK_SUCCESS)
    return false;

  _renderer.initialize_with_surface(_instance, _device, surface, _physical_device);
  return true;
}
