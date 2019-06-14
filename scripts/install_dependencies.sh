#!/bin/bash

sudo apt-get remove  -y llvm llvm-tools llvm-runtime llvm-dev
sudo apt-get autoremove -y
sudo apt-get install -y g++ cmake
sudo apt-get install -y libc++-dev libc++abi-dev
sudo apt-get install -y llvm-6.0 llvm-6.0-dev llvm-6.0-runtime llvm-6.0-tools
sudo apt-get install -y bison flex