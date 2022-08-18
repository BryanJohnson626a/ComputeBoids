cmake_minimum_required (VERSION 3.8)

add_executable (ComputeBoids)

target_compile_features(ComputeBoids PRIVATE
  cxx_std_20
  cxx_aggregate_default_initializers
)

target_include_directories(ComputeBoids
  PUBLIC ${VULKAN_INCLUDE_DIRS})

target_link_libraries(ComputeBoids
  Vulkan::Vulkan
  glfw
)

add_subdirectory (Engine)
add_subdirectory (Application)
