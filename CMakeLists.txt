cmake_minimum_required(VERSION 3.28.0)

project(LazyFoo_tutorials VERSION 1.0)

add_executable(${CMAKE_PROJECT_NAME} src/main.cpp)

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3)
target_link_libraries(${CMAKE_PROJECT_NAME} SDL3::SDL3)
