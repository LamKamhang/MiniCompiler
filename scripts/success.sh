#!/bin/bash

function test(){
	for element in `ls $1`
	do
		dir_or_file=$1"/"$element
		if [ -d $dir_or_file ]; then
			test $dir_or_file
		else
			if [[ `./build/release/ncc $dir_or_file` != "ok" ]]; then
				exit 1
			fi
		fi
	done
}

test_dir="test"

test $test_dir
echo "pass parser test"