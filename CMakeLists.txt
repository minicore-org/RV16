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

include_directories(${CMAKE_SOURCE_DIR}/include)

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

# clang-format
find_program(CLANG_FORMAT clang-format REQUIRED)

file(GLOB_RECURSE ALL_SOURCE_FILES
  ${CMAKE_SOURCE_DIR}/lib/*.cc
  ${CMAKE_SOURCE_DIR}/lib/*.h
  ${CMAKE_SOURCE_DIR}/include/*.h
  ${CMAKE_SOURCE_DIR}/emulator/*.cc
  ${CMAKE_SOURCE_DIR}/tests/*.cc
)

add_custom_target(format
  COMMAND ${CLANG_FORMAT} -i ${ALL_SOURCE_FILES}
  COMMENT "Running clang-format..."
)

# Google tests
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)
FetchContent_MakeAvailable(googletest)

enable_testing() 
add_subdirectory(tests)

add_custom_target(run-tests
  COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
  DEPENDS elf-parser-tests
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)