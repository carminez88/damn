cmake_minimum_required(VERSION 3.20)

project(Damn LANGUAGES CXX)

option(DAMN_BUILD_TESTS "Build tests" OFF)

add_subdirectory(common)
add_subdirectory(client)
add_subdirectory(broker)

if(DAMN_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()
