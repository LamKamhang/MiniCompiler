#!/bin/bash

if [ $# -eq 0 ]; then
	echo "unknow test target!"
elif [ $# -eq 1 ]; then
	if [ $1 == "lex" ]; then
		cd src
		flex scanner.lex
		if [ $? -eq 0 ]; then
			gcc lex.yy.c
			if [ $? -eq 0 ]; then
				echo "ok"
				./a.out
			fi
		fi
	fi
fi