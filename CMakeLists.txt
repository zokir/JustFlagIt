cmake_minimum_required(VERSION 3.18)

project(bibi VERSION 1.0 LANGUAGES CXX)

# SWIG thing
if(POLICY CMP0078)
  cmake_policy(SET CMP0078 NEW)
endif()

option(BUILD_CXX "Build C++" ON)
option(BUILD_PYTHON "Build Python" ON)
option(BUILD_JAVA "Build JAVA" ON)

set(CMAKE_SWIG_FLAGS)
find_package(SWIG REQUIRED)
include(UseSWIG)

file(GLOB sources "src/*.cc")
set_property(SOURCE bibi.i PROPERTY CPLUSPLUS ON)

# Python --------------------------------------------------
find_package(Python3 REQUIRED COMPONENTS Interpreter Development.Module)
#list(APPEND CMAKE_SWIG_FLAGS "-py3" "-DPY3")
set(PROJECT_NAME_PYTHON ${PROJECT_NAME}_python)
swig_add_library(${PROJECT_NAME_PYTHON}
    TYPE MODULE
    LANGUAGE python
    SOURCES bibi.i ${sources}
    OUTFILE_DIR ${CMAKE_INSTALL_PREFIX}/python
    OUTPUT_DIR ${CMAKE_INSTALL_PREFIX}/python
)
target_include_directories(${PROJECT_NAME_PYTHON} SYSTEM PRIVATE ${Python3_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME_PYTHON} Python3::Module)
# ---------------------------------------------------------

# Java ----------------------------------------------------
# TODO: Check if java found, else use JAVA_HOME env.
set(JAVA_HOME $ENV{JAVA_HOME})
find_package(Java)
find_package(JNI)
set(PROJECT_NAME_JAVA ${PROJECT_NAME}_java)
swig_add_library(${PROJECT_NAME_JAVA}
    TYPE MODULE
    LANGUAGE java 
    SOURCES bibi.i ${sources}
    OUTFILE_DIR ${CMAKE_INSTALL_PREFIX}/java/com/zok/panda/
    OUTPUT_DIR ${CMAKE_INSTALL_PREFIX}/java/com/zok/panda/
)
set_property(TARGET ${PROJECT_NAME_JAVA} PROPERTY SWIG_COMPILE_OPTIONS -package com.zok.panda)
message(WARNING "LOL ${JAVA_INCLUDE_PATH}")
target_include_directories(${PROJECT_NAME_JAVA} SYSTEM PRIVATE "$ENV{JAVA_HOME}/include" "$ENV{JAVA_HOME}/include/darwin")
swig_link_libraries(${PROJECT_NAME_JAVA} ${JAVA_LIBRARIES})
# ---------------------------------------------------------

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/include/")

option(BUILD_SHARED_LIBS ON)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME_JAVA} DESTINATION java/lib/)
install(TARGETS ${PROJECT_NAME_PYTHON} DESTINATION python/)
