#!/bin/bash
#This is the file which runs the commands in order to get the output
gcc -o exe lin.c
./tracescript.sh
gcc -w -o out cache.c getad.c d1.c
./out
