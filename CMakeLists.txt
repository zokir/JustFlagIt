cmake_minimum_required(VERSION 3.18)

project(flagit VERSION 1.0 LANGUAGES CXX)

# Fix, exception specifications with wrappers.
set(CMAKE_CXX_STANDARD 14)

# SWIG thing
if(POLICY CMP0078)
  cmake_policy(SET CMP0078 NEW)
endif()

# TODO: We need to use these to enter into subdirs
option(BUILD_CXX "Build C++" ON)
message(STATUS "Build C++: ${BUILD_CXX}")
option(BUILD_PYTHON "Build Python" ON)
message(STATUS "Build Python: ${BUILD_PYTHON}")

option(BUILD_TESTS "Build TESTS" ON)
message(STATUS "Build TESTS: ${BUILD_TESTS}")

set(CMAKE_SWIG_FLAGS)
find_package(SWIG REQUIRED)
include(UseSWIG)

file(GLOB sources "src/*.cc")
set_property(SOURCE flagit.i PROPERTY CPLUSPLUS ON)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/")

option(BUILD_SHARED_LIBS ON)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

find_package(CURL REQUIRED) 
include_directories(${CURL_INCLUDE_DIR})

find_package(nlohmann_json REQUIRED)

if (BUILD_PYTHON)
  set(PROJECT_NAME_PYTHON "${PROJECT_NAME}_python")
  find_package(Python3 REQUIRED COMPONENTS Interpreter Development.Module)
  list(APPEND CMAKE_SWIG_FLAGS "-py3" "-DPY3")
  swig_add_library(${PROJECT_NAME_PYTHON}
      TYPE MODULE
      LANGUAGE python
      SOURCES ${CMAKE_SOURCE_DIR}/flagit.i ${sources}
      OUTFILE_DIR ${CMAKE_INSTALL_PREFIX}/python
      OUTPUT_DIR ${CMAKE_INSTALL_PREFIX}/python
  )

  target_include_directories(${PROJECT_NAME_PYTHON} SYSTEM PRIVATE ${Python3_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME_PYTHON} PUBLIC Python3::Module ${CURL_LIBRARIES} nlohmann_json::nlohmann_json)

  install(TARGETS ${PROJECT_NAME_PYTHON} DESTINATION python/)

endif() # BUILD_PYTHON

if (BUILD_CXX)
  set(PROJECT_NAME_CXX "${PROJECT_NAME}_cxx")
  add_library(${PROJECT_NAME_CXX} SHARED ${sources} src/FileUtils.cc src/FileUtils.hh)
  target_link_libraries(${PROJECT_NAME_CXX} PUBLIC ${CURL_LIBRARIES} nlohmann_json::nlohmann_json)

  set_target_properties(${PROJECT_NAME_CXX} PROPERTIES PUBLIC_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/src/flagit.hh")
  install(TARGETS ${PROJECT_NAME_CXX}
          LIBRARY
            DESTINATION cpp/lib
          PUBLIC_HEADER
            DESTINATION cpp/include)

endif() # BUILD_CPP

if (BUILD_TESTS)
  include(CTest)
  enable_testing()
  set(CMAKE_MACOSX_RPATH 1)
  set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/cpp/lib")

  find_package(Catch2 3 REQUIRED)
  include(Catch)

  add_executable(test_basic tests/test_basic.cc tests/MockDataFetcher.cc tests/MockDataFetcher.hh)
  add_test(NAME test_basic COMMAND $<TARGET_FILE:test_basic>)
  target_include_directories(test_basic PRIVATE ${Catch2_INCLUDE_DIR})
  target_link_libraries(test_basic PRIVATE ${PROJECT_NAME_CXX} Catch2::Catch2WithMain)
    
  # TODO: Figure out how to make this work, order of C++ install might mess this up
  #add_custom_command(TARGET test_basic 
  #                   POST_BUILD
  #                   COMMAND ctest -C $<CONFIGURATION> --output-on-failure)
endif() # BUILD_TESTS

