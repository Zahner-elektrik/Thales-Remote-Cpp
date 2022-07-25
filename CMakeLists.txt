cmake_minimum_required(VERSION 3.5)

project(ThalesRemoteCpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(ThalesRemoteCppLibrary)
add_subdirectory(ThalesRemoteExternalLibrary)
add_subdirectory(GeneralExample)
add_subdirectory(EisPad4Example)
add_subdirectory(LibraryDocumentation)
add_subdirectory(InterneTests)
add_subdirectory(FileExchangeExample)
add_subdirectory(EisDLLExample)
add_subdirectory(ExternalDeviceFRA)
add_subdirectory(DCSequencerExample)

file(GLOB_RECURSE GitHubFiles Readme.md LICENSE)
add_custom_target(GitHubFiles SOURCES ${GitHubFiles})
