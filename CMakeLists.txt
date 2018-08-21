cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

set(library_name libyacl)

set(CMAKE_CXX_STANDARD 11)

project(${library_name})

set(library_obj ${library_name}_obj)
set(library_static ${library_name}_static)
set(library_shared ${library_name}_shared)

# get all source files from src
file(GLOB_RECURSE source ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# this is the "object library" target: compiles the sources only once
add_library(${library_obj} OBJECT ${source})

# shared libraries need PIC
set_property(TARGET ${library_obj} PROPERTY POSITION_INDEPENDENT_CODE 1)

# shared and static libraries built from the same object files
add_library(${library_shared} SHARED $<TARGET_OBJECTS:${library_obj}>)
add_library(${library_static} STATIC $<TARGET_OBJECTS:${library_obj}>)


target_include_directories(${library_obj} PUBLIC 
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

target_include_directories(${library_static} PUBLIC 
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

target_include_directories(${library_shared} PUBLIC 
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)



target_include_directories(${library_obj} PRIVATE 
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

add_subdirectory(examples/)