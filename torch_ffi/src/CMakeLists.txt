# The Flutter tooling requires that developers have CMake 3.10 or later
# installed. You should not increase this version, as doing so will cause
# the plugin to fail to compile for some customers of the plugin.
cmake_minimum_required(VERSION 3.10)

project(torch_ffi_library VERSION 0.0.1 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

message(STATUS "==CMAKE START==")

add_library(torch_ffi SHARED
  "torch_ffi.c"
)

set_target_properties(torch_ffi PROPERTIES
  PUBLIC_HEADER "torch_ffi.h"
  OUTPUT_NAME "torch_ffi"
)

target_compile_definitions(torch_ffi PUBLIC DART_SHARED_LIB)

message(STATUS "==CMAKE DONE==")
