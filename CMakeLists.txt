cmake_minimum_required(VERSION 3.30)
project(KernelImageProcessor)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(test)

set(SOURCE_FILES PixelImage.cpp
        PixelImage.h
        PNMFileHandler.cpp
        PNMFileHandler.h
        Image.cpp
        Image.h
        ImageEditor.cpp
        ImageEditor.h
        Kernel.cpp
        Kernel.h
        KernelMode.h)

add_executable(KernelImageProcessor main.cpp)
add_library(core ${SOURCE_FILES})
target_link_libraries(KernelImageProcessor core)


