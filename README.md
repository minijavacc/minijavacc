Compiler
========

of group 5 at Compilerpraktikum WS 2016/2017

## Directory structure
```
├── bin          - the build system creates the executable "compiler" in this directory
├── src
│   ├── main     - contains main function
│   ├── compiler - base class calling the single stages
│   ├── lexer    \
│   └── parser   / compiler pipeline stages
│   
└── test
```

## Building
```
$ cmake ./
$ make
```

or just run ```build.sh``` in the base directory. 
