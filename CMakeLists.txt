cmake_minimum_required(VERSION 3.30)
project(KernelImageProcessor)

set(CMAKE_CXX_STANDARD 11)

add_executable(KernelImageProcessor main.cpp
        PixelImage.cpp
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
