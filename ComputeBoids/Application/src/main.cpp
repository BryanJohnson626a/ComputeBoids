#include <iostream>

#include "../../Engine/include/engine.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


int main(void)
{
  glfwInit();

  if (!glfwVulkanSupported())
  {
    std::cout << "Vulkan not supported by GLFW." << std::endl;
    return 1;
  }

  uint32_t count{};
  const char * const * GLFWExtensions = glfwGetRequiredInstanceExtensions(&count);
  std::vector<const char *> extensions;
  extensions.assign(GLFWExtensions, GLFWExtensions + count);

  Engine engine(extensions);

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  GLFWwindow * window = glfwCreateWindow(640, 480, "Window Title", NULL, NULL);

  //bool quit = false;
  //while (!quit)
  //{
  //}

  system("pause");

  return 0;
}
