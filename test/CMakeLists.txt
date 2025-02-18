cmake_minimum_required(VERSION 3.30)

include(FetchContent)
FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG main
)
FetchContent_MakeAvailable(googletest)


set(TEST_SOURCE_FILES runAllTests.cpp
        PNMImageHandlerFixture.cpp
        KernelFixture.cpp)
add_executable(runKernelImageProcessorTests ${TEST_SOURCE_FILES})
target_link_libraries(runKernelImageProcessorTests gtest gtest_main core)


set(DATA_DIR "${CMAKE_SOURCE_DIR}/test/data")

add_custom_target(copy_data ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${DATA_DIR}
        ${CMAKE_BINARY_DIR}/test/test/data
)

add_dependencies(runKernelImageProcessorTests copy_data)

