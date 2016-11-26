project(minijavacc)
cmake_minimum_required(VERSION 2.8.7)

set(CMAKE_BINARY_DIR bin)
set(CMAKE_SOURCE_DIR src)
set(CMAKE_LIBRARY_PATH lib)
set(EXECUTABLE_OUTPUT_PATH bin)

# get current working branch from git
execute_process(
  COMMAND git rev-parse --abbrev-ref HEAD
  WORKING_DIRECTORY . 
  OUTPUT_VARIABLE GIT_BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# add libFirm
add_custom_target(libfirm
  COMMAND $(MAKE) --silent
  WORKING_DIRECTORY lib/libfirm
  COMMENT Building libfirm
)

include_directories(
  src/main
  src/compiler
  src/lexer
  src/stringtable
  src/parser
  src/semantics
  src/symboltable
  src/graphcreator
  lib/libfirm/include
  lib/libfirm/build/gen/include/libfirm
)

link_directories(
  lib/libfirm/build/debug
)

# source files
file(GLOB_RECURSE compiler_SRCS
  "src/*.hpp"
  "src/*.cpp"
)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

if(GIT_BRANCH STREQUAL "master")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O3 -DNDEBUG")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O0 -DDEBUG -g")
endif()


add_executable(minijavacc ${compiler_SRCS})
add_dependencies(minijavacc libfirm)
target_link_libraries(minijavacc LINK_PUBLIC firm)
