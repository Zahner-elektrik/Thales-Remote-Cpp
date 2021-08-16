cmake_minimum_required(VERSION 3.5)

project(EisPad4Example)

add_executable (EisPad4Example main.cpp)
target_link_libraries (EisPad4Example PRIVATE ThalesRemoteCppLibrary)
if(WIN32)
  target_link_libraries(EisPad4Example PRIVATE wsock32 ws2_32)
endif()
