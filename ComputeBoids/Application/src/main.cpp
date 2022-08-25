#include <iostream>
#include <vector>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "../../Engine/src/engine.hpp"

int main(void)
{
  glfwInit();

  if (!glfwVulkanSupported())
  {
    std::cout << "Vulkan not supported by GLFW." << std::endl;
    return 1;
  }

  uint32_t count{};
  const char *const *GLFWExtensions = glfwGetRequiredInstanceExtensions(&count);
  std::vector<const char *> extensions;
  extensions.assign(GLFWExtensions, GLFWExtensions + count);

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  GLFWwindow * window = glfwCreateWindow(640, 480, "Window Title", NULL, NULL);

  Engine engine(extensions);

  engine.create_renderer_from_window(window);

  std::cout << "created renderer successfully" << std::endl;

 // bool quit = false;
  //while (!quit)
  //{
  //}

  system("pause");

  return 0;
}
