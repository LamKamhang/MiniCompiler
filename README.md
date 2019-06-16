# MiniCompiler
[![Build Status](https://travis-ci.com/LamKamhang/MiniCompiler.svg?token=5hSCR1UysbxGz9FPbrxj&branch=master)](https://travis-ci.com/LamKamhang/MiniCompiler)

A simple C-style Language Compiler.

## To Do List

- [x] Produce a parser for a subset of C
- [x] Generate LLVM-IR and target code
- [ ] C language characteristics
    - [ ] Basic type
        - [x] (bool), char, int, unsgined, long ...
        - [x] float, double
        - [ ] void
  - [x] control flow
  - [ ] loop
  - [x] assignment expression
  - [ ] colon expression
  - [ ] question expression
  - [x] basic operations: +, -, *, /
  - [ ] logical operations: &&, ||
  - [ ] bit operation: <<, >>, |, &
  - [ ] compare: >, ==, < , !=, >=, <=
  - [x] function call
  - [x] unspecified parameter
  - [x] auto format transformation
  - [x] const
  - [ ] static, extern
  - [x] variable definition
  - [ ] pointer type 
  - [ ] dereference: '*', '.', '->'
  - [ ] array type
  - [ ] struct and union
  - [ ] list initialization
- [x] Error check
- [ ] Support including standard library and macros

## Contributors

- Lin Jinkeng
- Lin Candong
- Xu Hongjia


## problem in parser
* duplicate const and unsigned warning
* remove '()' and '[]'
