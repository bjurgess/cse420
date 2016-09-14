#!/bin/bash
LD_LIBRARY_PATH="/user/cse420/Public/lib"
export LD_LIBRARY_PATH

echo "%mMulti_CPLocks" >> CPLocks.txt
for ((i=1; i<=20; i++))
do
	echo "Running with $i threads."
	mMulti_CPLocks $i >> CPLocks.txt
done
