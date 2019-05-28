#!/bin/bash

if [ $# -eq 0 ]; then
	cmake . -Bbuild/debug -DCMAKE_BUILD_TYPE=Debug
	if [ $? -eq 0 ]; then
		cd build/debug
		make
	fi
elif [ $# -eq 1 ]; then
    if [ $1 == "profile" ]; then
	cmake . -Bbuild/debug -DCMAKE_BUILD_TYPE=Debug -DPROFILE_FLAG=1
	if [ $? -eq 0 ]; then
	    cd build/debug
	    make
	fi
    elif [ $1 == "release" ]; then
	cmake . -Bbuild/release -DCMAKE_BUILD_TYPE=Release
	if [ $? -eq 0 ]; then
	    cd build/release
	    make
	fi
    else
	echo "unknown argument"
    fi
elif [ $# -eq 2 ]; then
    if [ $1 == "release" ]; then
	if [ $2 == "profile" ]; then
	    cmake . -Bbuild/release -DCMAKE_BUILD_TYPE=Release -DPROFILE_FLAG=1
	    if [ $? -eq 0 ]; then
		cd build/release
		make
	    fi
	else
	    echo "unknown argument"
	fi
    else
	echo "unknown argument"
    fi
else
	echo "build failed!"
fi
