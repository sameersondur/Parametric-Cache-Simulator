#!/bin/bash
#This is the file to get the tracefile
valgrind --tool=lackey --trace-mem=yes exe >& out
sed '/==/d' out > out1
sed '/I/d' out1 > out22
sed '/M/d' out22 > out2
sed 's/,/ /' out2 > out3
sed 's/ S /S /' out3 >out2
sed 's/ L /L /' out2 >out3
sed 's/ M /M /' out3 >out4
cat out4 | awk '{ print $1" "$2 }' > tracefile
rm out1 out2 out3 out out22 out4
