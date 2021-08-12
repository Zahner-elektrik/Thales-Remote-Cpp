cmake_minimum_required(VERSION 3.5)

project(GeneralExample)

add_executable (GeneralExample main.cpp)
target_link_libraries (GeneralExample PRIVATE ThalesRemoteCppLibrary)
if(WIN32)
  target_link_libraries(GeneralExample PRIVATE wsock32 ws2_32)
endif()
