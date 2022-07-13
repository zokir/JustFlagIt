cmake_minimum_required(VERSION 3.18)

project(flagit VERSION 1.0 LANGUAGES CXX)

# SWIG thing
if(POLICY CMP0078)
  cmake_policy(SET CMP0078 NEW)
endif()

# TODO: We need to use these to enter into subdirs
option(BUILD_CXX "Build C++" ON)
option(BUILD_PYTHON "Build Python" ON)
message(STATUS "Build Python: ${BUILD_PYTHON}")

set(CMAKE_SWIG_FLAGS)
find_package(SWIG REQUIRED)
include(UseSWIG)

file(GLOB sources "src/*.cc")
set_property(SOURCE flagit.i PROPERTY CPLUSPLUS ON)

if (BUILD_PYTHON)
  add_subdirectory(python)
endif()

option(BUILD_SHARED_LIBS ON)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
