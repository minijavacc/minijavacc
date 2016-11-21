project(compiler)
cmake_minimum_required(VERSION 2.8.7)

set(CMAKE_BINARY_DIR bin)
set(CMAKE_SOURCE_DIR src)
set(EXECUTABLE_OUTPUT_PATH bin)

include_directories(
  src
  src/main
  src/compiler
  src/lexer
  src/stringtable
  src/parser
  src/semantics
  src/symboltable
)

# source files
file(GLOB_RECURSE compiler_SRCS
  "src/*.hpp"
  "src/*.cpp"
)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

add_executable (compiler ${compiler_SRCS})
