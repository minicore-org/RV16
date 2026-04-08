add_library(rv16core
  elf-parser.cc
)

target_include_directories(rv16core PUBLIC ${CMAKE_SOURCE_DIR}/include)
