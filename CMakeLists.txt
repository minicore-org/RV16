cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_COMPILER clang++)

project(miniCore
  VERSION 0.1.0
  DESCRIPTION "RV16 open architecture t design and toolchain"
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-stdlib=libc++)
add_link_options(-stdlib=libc++)

# Default to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

add_compile_options(
  -Wall
  -Wextra
  -Wpedantic
  #-Werror
  $<$<CONFIG:Debug>:-g>
  $<$<CONFIG:Debug>:-O0>
  $<$<CONFIG:Release>:-O2>
)

add_subdirectory(lib)
add_subdirectory(emulator)