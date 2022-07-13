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

option(BUILD_SHARED_LIBS ON)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
install(TARGETS ${PROJECT_NAME} DESTINATION python/)