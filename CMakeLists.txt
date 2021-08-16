cmake_minimum_required(VERSION 3.5)

project(ThalesRemoteCpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(ThalesRemoteCppLibrary)
add_subdirectory(GeneralExample)
add_subdirectory(EisPad4Example)

file(GLOB_RECURSE GitHubFiles Readme.md LICENSE)
add_custom_target(GitHubFiles SOURCES ${GitHubFiles})
