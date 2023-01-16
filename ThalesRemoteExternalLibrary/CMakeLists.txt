cmake_minimum_required(VERSION 3.5)

project(ThalesRemoteExternalLibrary)

file(GLOB_RECURSE ExternalLibrarySrc
    "*.h"
    "*.cpp"
)

add_library (ThalesRemoteExternalLibrary SHARED ${ExternalLibrarySrc})

target_include_directories (ThalesRemoteExternalLibrary PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries (ThalesRemoteExternalLibrary PRIVATE ThalesRemoteCppLibrary -static)
if(WIN32)
  target_link_libraries(ThalesRemoteExternalLibrary PRIVATE wsock32 ws2_32 -static)
endif()
 
