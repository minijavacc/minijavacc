project(compiler)
cmake_minimum_required(VERSION 2.8.7)

set(CMAKE_BINARY_DIR bin)
set(CMAKE_SOURCE_DIR src)
set(EXECUTABLE_OUTPUT_PATH bin)

include_directories(
  src
  src/main
  src/compiler
  src/parser
  src/lexer
  src/stringtable
)

# source files
file(GLOB_RECURSE compiler_SRCS
  "src/*.hpp"
  "src/*.cpp"
)

set(CMAKE_CXX_STANDARD 11)

add_executable (compiler ${compiler_SRCS})
