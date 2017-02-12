minijavacc
==========

[![Build Status](https://travis-ci.org/minijavacc/minijavacc.svg?branch=develop)](https://travis-ci.org/minijavacc/minijavacc)

*minijavacc* is a compiler for MiniJava, a subset of Java, that generates x86_64 assembler code. 
It was developed for the compiler lab in winter term 16/17 at [Karlsruher Institute of Technolgy (KIT)](http://www.kit.edu). 

It uses the [libfirm](http://pp.ipd.kit.edu/firm) library for graph-based intermediate representation of the input program. 

This compiler in comparison to the other teams: [speedcenter.informatik.kit.edu](http://speedcenter.informatik.kit.edu/)

## Directory structure
```
├── bin
├── hs                 - some parts have also been implemented in Haskell
├── lib
│   └── libfirm        - library for graph-based intermediate representation
├── meta
│   ├── presentation   - slides used for lab presentation
│   └── specs          - specification of AST
├── src
│   ├── backend        - code generation
│   ├── compiler       - compiler base class
│   ├── graphcreator   - creation of ir-graph (for libfirm)
│   ├── lexer          - lexer
│   ├── main           - main function
│   ├── optimizer      - graph optimization
│   ├── parser         - parser
│   ├── semantics      - semantic check (name and type analysis)
│   ├── structures     - AST classes
│   └── tests
├── tests
│   ├── compilertester
│   ├── lexer
│   ├── mjtest         - test suite used in lab
│   └── unittests      - single test cases
└── tools
    └── yComp-1.3.19   - helpful tool for viewing ir-graph
```

## Setup
After cloning the main repository the submodules libfirm and mjtest must be cloned, too. 
```
$ git submodule init
$ git submodule update
```

## Build
The build automatically builds all needed libraries and the main executable itsels. The executable then can be found at `./bin/minijavacc`. 
```
$ ./build
```

## Usage
```
$ ./bin/minijavacc tests/unittests/simple_add.java
all semantic checks passed
Created assembler file: asm.s
Created binary: a.out
$ ./a.out 
7
```
