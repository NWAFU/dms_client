#!/bin/bash

if cat dev/null > $1
then
	exit 0 # clean up file sucessful
else
	exit -1 # clearn up file failed.
