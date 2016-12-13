project(minijavacc)
cmake_minimum_required(VERSION 2.8.7)

set(CMAKE_BINARY_DIR bin)
set(CMAKE_SOURCE_DIR src)
set(CMAKE_LIBRARY_PATH lib)
set(EXECUTABLE_OUTPUT_PATH bin)

# read runtime library into variable
#file (STRINGS "runtime/runtime.c" RUNTIME_SOURCE)
#string(REPLACE "\"" "\\\"" RUNTIME_SOURCE ${RUNTIME_SOURCE})
#configure_file(src/graphcreator/runtimesource.h.in src/graphcreator/runtimesource.h)

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
  src/backend
  lib/libfirm/include
  lib/libfirm/build/gen/include/libfirm
)

ADD_LIBRARY(firm STATIC IMPORTED)
SET_TARGET_PROPERTIES(firm PROPERTIES
  IMPORTED_LOCATION lib/libfirm/build/debug/libfirm.a)

# source files
file(GLOB_RECURSE compiler_SRCS
  "src/*.hpp"
  "src/*.cpp"
)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -std=c++14 -O0 -DDEBUG -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -std=c++14 -O3 -DNDEBUG")


add_executable(minijavacc ${compiler_SRCS})
add_dependencies(minijavacc libfirm)
target_link_libraries(minijavacc LINK_PUBLIC firm)
