#!/bin/bash

./build.sh release

if [ $? -ne 0 ]; then
	echo "build failed"
	exit 1
fi

exit_code=0
function test(){
	for element in `ls $1`
	do
		dir_or_file=$1"/"$element
		if [ -d $dir_or_file ]; then
			test $dir_or_file
		else
			filename=${dir_or_file%.*}
			extension=${dir_or_file##*.}
			if [[ $extension == "c" ]]; then
				./build/release/ncc $dir_or_file > _res
				if [[ `diff _res $filename.res` != "" ]]; then
					echo "$filename failed."
					exit_code=2
				fi
			fi
		fi
	done
}

test_dir="test"

test $test_dir

if [ $exit_code -ne 0 ]; then
	exit $exit_code
else
	echo "pass parser test"
	exit 0
fi