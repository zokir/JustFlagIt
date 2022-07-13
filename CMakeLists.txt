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
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/")
option(BUILD_SHARED_LIBS ON)

if (BUILD_PYTHON)

  find_package(Python3 REQUIRED COMPONENTS Interpreter Development.Module)
  list(APPEND CMAKE_SWIG_FLAGS "-py3" "-DPY3")
  swig_add_library(${PROJECT_NAME}
      TYPE MODULE
      LANGUAGE python
      SOURCES ${CMAKE_SOURCE_DIR}/flagit.i ${sources}
      OUTFILE_DIR ${CMAKE_INSTALL_PREFIX}/python
      OUTPUT_DIR ${CMAKE_INSTALL_PREFIX}/python
  )

  target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${Python3_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME} Python3::Module)

  set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
  install(TARGETS ${PROJECT_NAME} DESTINATION python/)

endif() # BUILD_PYTHON
