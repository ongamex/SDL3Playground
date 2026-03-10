cmake_minimum_required(VERSION 3.14)
project(AppSDL3Test)

set(CMAKE_CXX_STANDARD 20)

#cmake_path(GET CMAKE_CURRENT_SOURCE_DIR PARENT_PATH MY_PARENT_DIR)
#cmake_path(GET CMAKE_CURRENT_SOURCE_DIR ABSOLUTE_PATH MY_PARENT_DIR)

set(MY_DXC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/dxc/dxc.exe)
set(MY_GLSLC_PATH "C:/VulkanSDK/1.2.170.0/Bin/glslc.exe")
set(MY_SLANG_PATH ${CMAKE_CURRENT_SOURCE_DIR}/slang/bin/slangc.exe)

message("MY_DXC_PATH = ${MY_DXC_PATH}")
message("MY_SLANG_PATH = ${MY_SLANG_PATH}")

add_subdirectory(SDL)
add_subdirectory(app)
