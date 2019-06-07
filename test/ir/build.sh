#!/bin/bash
dir="build"
flag="-DCMAKE_BUILD_TYPE=Debug"
cmake . -B${dir} ${flag}
cd ${dir}
make