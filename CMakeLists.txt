cmake_minimum_required(VERSION 3.28.0)
project(LazyFoo_tutorials VERSION 1.0)

add_library(compiler_flags INTERFACE)
target_compile_features(compiler_flags INTERFACE cxx_std_20)

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

target_compile_options(compiler_flags INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
  "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

add_executable(${CMAKE_PROJECT_NAME} src/main.cpp)

include_directories(assets)

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3)
find_package(SDL3_image REQUIRED CONFIG REQUIRED COMPONENTS SDL3_image)
find_package(SDL3_ttf REQUIRED CONFIG REQUIRED COMPONENTS SDL3_ttf)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL3::SDL3 SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf compiler_flags)
