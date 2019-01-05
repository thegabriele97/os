#!/bin/bash

if ! [ $# -eq 3 ]; then
	exit 1
fi

old_ifs=$IFS
new_ifs="
"

i=0
IFS=$new_ifs
for file in $(ls -l $3 | sort -k5gr,5gr); do
	if [ $i -ge $1 ]; then
		break
	fi

	real=$(echo $file | cut -f9 -d' ')
	$2 $3/$real

	i=$[$i+1]
done
