cmake_minimum_required(VERSION 3.17)
project(CCom)

set(CMAKE_CXX_STANDARD 20)

enable_testing()

include_directories(src)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(lib/googletest)