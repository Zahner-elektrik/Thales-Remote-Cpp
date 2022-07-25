cmake_minimum_required(VERSION 3.5)

project(DCSequencerExampleExample)

add_executable (DCSequencerExampleExample main.cpp)

target_link_libraries (DCSequencerExampleExample PRIVATE ThalesRemoteCppLibrary)
if(WIN32)
  target_link_libraries(DCSequencerExampleExample PRIVATE wsock32 ws2_32)
endif()
