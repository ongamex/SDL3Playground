cmake_minimum_required(VERSION 3.14)
project(AppSDL3Test)

set(MY_DXC_PATH ${CMAKE_LISTS_DIR}/dxc/dxc.exe)
set(MY_GLSLC_PATH "C:/VulkanSDK/1.2.170.0/Bin/glslc.exe")


add_subdirectory(SDL)
add_subdirectory(app)
