#include <iostream>

#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

int main(void)
{

  if (glfwVulkanSupported())
  {
    std::cout << "Vulkan Supported!" << std::endl;
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow * window = glfwCreateWindow(640, 480, "Window Title", NULL, NULL);
  }

  std::cout << "Program finished." << std::endl;
  system("pause");
  return 0;
}
