# Top-level Cmake

cmake_minimum_required(VERSION 3.8)

set(HUNTER_ROOT "/cmake")
include("cmake/HunterGate.cmake")

HunterGate(
  URL "https://github.com/cpp-pm/hunter/archive/v0.24.3.tar.gz"
  SHA1 "10738b59e539818a01090e64c2d09896247530c7" 
)

project("ComputeBoids"
  LANGUAGES CXX
)

hunter_add_package(glfw)

find_package(Vulkan REQUIRED FATAL_ERROR)
find_package(glfw3 REQUIRED)

add_subdirectory("ComputeBoids")

