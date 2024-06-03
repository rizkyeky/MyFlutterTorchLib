# The Flutter tooling requires that developers have CMake 3.10 or later
# installed. You should not increase this version, as doing so will cause
# the plugin to fail to compile for some customers of the plugin.
cmake_minimum_required(VERSION 3.10)

project(torch_cpp_library VERSION 0.0.1 LANGUAGES C)

add_library(torch_cpp SHARED
  "torch_cpp.c"
)

set_target_properties(torch_cpp PROPERTIES
  PUBLIC_HEADER torch.h
  OUTPUT_NAME "torch_cpp"
)

target_compile_definitions(torch_cpp PUBLIC DART_SHARED_LIB)
