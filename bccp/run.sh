#!/bin/bash
for((c=1; c<=$1; c++))
do
	./bccp -c $2 -m $3
done
