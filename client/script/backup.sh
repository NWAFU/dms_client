#!/bin/bash

time=$(date "+%Y%m%d%H%M%S")
if cp $1 $1.$time
then
	if cat /dev/null > $1	
	then
		echo $1.$time > name.txt	
		exit 0
	else
		exit 2
	fi
else
	exit 1
fi
 
