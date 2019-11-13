#!/bin/bash	
 
## 파라미터가 없으면 종료 
if [ "$#" -lt 1 ]; then
	echo "$# is Illegal number of parameters."
	echo "Usage: $0 [options]"
	exit 1
fi


# -o : or
if [ "$1" == "--daemon" -o "$1" == "-d" ]; then
	echo "aaaaaaa"
	exit 1
fi
