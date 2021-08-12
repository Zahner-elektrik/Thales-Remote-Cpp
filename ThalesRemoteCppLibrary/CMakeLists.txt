cmake_minimum_required (VERSION 3.5)

project(ThalesRemoteCppLibrary)

add_library(ThalesRemoteCppLibrary thalesremoteconnection.cpp thalesremotescriptwrapper.cpp)
target_include_directories (ThalesRemoteCppLibrary PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
