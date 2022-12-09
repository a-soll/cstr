cmake_minimum_required(VERSION 3.0)
set(CMAKE_C_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER ${CMAKE_CXX_COMPILER})

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

project(cstr VERSION 1.0)
include(GNUInstallDirs)

add_library(cstr cstr.c)

set_target_properties(cstr PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(cstr PROPERTIES PUBLIC_HEADER cstr.h)

install(TARGETS cstr LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/cstr)
