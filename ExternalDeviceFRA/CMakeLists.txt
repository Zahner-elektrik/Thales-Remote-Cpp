cmake_minimum_required(VERSION 3.5)

project(ExternalDeviceFRA)

add_executable (ExternalDeviceFRA main.cpp)
target_link_libraries (ExternalDeviceFRA PRIVATE ThalesRemoteCppLibrary)
if(WIN32)
  target_link_libraries(ExternalDeviceFRA PRIVATE wsock32 ws2_32)
endif()
