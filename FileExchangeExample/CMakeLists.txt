cmake_minimum_required(VERSION 3.5)

project(FileExchangeExample)

add_executable (FileExchangeExample main.cpp)
target_link_libraries (FileExchangeExample PRIVATE ThalesRemoteCppLibrary)
if(WIN32)
  target_link_libraries(FileExchangeExample PRIVATE wsock32 ws2_32)
endif()
