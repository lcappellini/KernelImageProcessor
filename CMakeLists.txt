cmake_minimum_required(VERSION 3.30)
project(KernelImageProcessor)

set(CMAKE_CXX_STANDARD 11)

add_executable(KernelImageProcessor main.cpp
        Image.cpp
        Image.h
        Channel.cpp
        Channel.h)
