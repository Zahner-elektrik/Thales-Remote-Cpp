cmake_minimum_required(VERSION 3.5)

project(EisDLLExample)

add_executable (EisDLLExample main.cpp)

if(WIN32)
    target_link_libraries(EisDLLExample PRIVATE wsock32 ws2_32 -static)
endif()
