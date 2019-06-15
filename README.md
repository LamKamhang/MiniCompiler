# MiniCompiler
[![Build Status](https://travis-ci.com/LamKamhang/MiniCompiler.svg?token=5hSCR1UysbxGz9FPbrxj&branch=master)](https://travis-ci.com/LamKamhang/MiniCompiler)

A simple C-style Language Compiler.

## To Do List

- [x] Produce a parser for a subset of C
- [x] Generate LLVM IR and target code
  - [x] control flow
  - [ ] loop
  - [ ] assignment expression
  - [ ] colon expression
  - [ ] question expression
  - [ ] basic operations: +, -, *, /
  - [ ] logical operations: &&, ||
  - [ ] bit operation: <<, >>, |, &
  - [ ] compare: >, ==, < , !=, >=, <=
  - [x] function call
  - [ ] unspecified parameter
  - [ ] auto format transformation
  - [ ] const, static, extern
  - [ ] variable definition
  - [ ] pointer type 
  - [ ] dereference: '*', '.', '->'
  - [ ] array type
  - [ ] struct and union
  - [ ] list initialization
- [x] Support including standard library and macros
- [ ] Error check

## Contributors

- Lin Jinkeng
- Lin Candong
- Xu Hongjia


## problem in parser
* duplicate const and unsigned warning
* remove '()' and '[]'
