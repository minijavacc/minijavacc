project(compiler)
cmake_minimum_required(VERSION 2.6)

include_directories(
  main
  compiler
)

# source files
set(compiler_SRCS
  main/main.cpp
  compiler/compiler.cpp
)

add_executable (../bin/compiler ${compiler_SRCS})
