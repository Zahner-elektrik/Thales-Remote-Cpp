cmake_minimum_required (VERSION 3.5)

project(ThalesRemoteCppLibrary)

add_library(ThalesRemoteCppLibrary
    thalesremoteconnection.cpp
    thalesremotescriptwrapper.cpp
    thalesremoteconnection.h
    thalesremotescriptwrapper.h
    zahnererror.cpp
    zahnererror.h
    thalesremoteerror.cpp
    thalesremoteerror.h
    termconnectionerror.cpp
    termconnectionerror.h
    threadsafequeue.cpp
    threadsafequeue.h
    thalesfileinterface.cpp
    thalesfileinterface.h)
target_include_directories (ThalesRemoteCppLibrary PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

