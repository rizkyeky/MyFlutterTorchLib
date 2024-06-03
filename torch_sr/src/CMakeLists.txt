cmake_minimum_required(VERSION 3.10)
project(TorchSR)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/Cellar/pytorch/2.0.1/share/cmake/")
find_package(Torch REQUIRED)
include_directories(${TORCH_INCLUDE_DIRS})

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

link_directories("${CMAKE_SOURCE_DIR}/lib")

add_library(torch_sr SHARED "torch_sr.cpp" "torch_sr.hpp")
target_link_libraries(torch_sr 
libc10.dylib
libtorch_cpu.dylib
libopencv_core.dylib
libopencv_imgcodecs.dylib
libopencv_imgproc.dylib
libopencv_dnn.dylib
)

set_target_properties(torch_sr PROPERTIES
    FRAMEWORK TRUE
    FRAMEWORK_VERSION CXX
    PUBLIC_HEADER "torch_sr.h"
    XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
)