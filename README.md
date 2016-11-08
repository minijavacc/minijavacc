minijavacc
==========

[![Build Status](https://travis-ci.org/minijavacc/minijavacc.svg?branch=develop)](https://travis-ci.org/minijavacc/minijavacc)

of team 5 at Compilerpraktikum WS 2016/2017

This compiler in comparison to the other teams: http://speedcenter.informatik.kit.edu/ 

## Directory structure
```
├── bin             - the build system creates the executable "compiler" in this directory
├── hs              - haskell fun
├── src
│   ├── main        - contains main function
│   ├── compiler    - base class calling the single stages
│   ├── lexer       \
│   ├── parser      / compiler pipeline stages
│   └── stringtable - string management
└── tests
```

## Setup
After cloning, run
```
$ git submodule init
$ git submodule update
```

## Build
```
$ ./build
```

## Build and Test
```
$ ./build_and_test
```
