cmake_minimum_required (VERSION 3.8)

add_library (VulkanEngine STATIC)
add_executable (ComputeBoids)

add_subdirectory (Engine)
add_subdirectory (Application)


target_compile_features(VulkanEngine PRIVATE
  cxx_std_20
  cxx_aggregate_default_initializers
)

target_link_libraries(VulkanEngine PUBLIC Vulkan::Vulkan)

target_include_directories(ComputeBoids PUBLIC ${ENGINE_INCLUDE})

target_link_libraries(ComputeBoids PRIVATE VulkanEngine
  glfw)

target_compile_features(ComputeBoids PRIVATE
  cxx_std_20
  cxx_aggregate_default_initializers
)


