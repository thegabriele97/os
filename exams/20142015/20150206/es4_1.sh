#!/bin/bash

if ! [ $# -eq 1 ]; then
	exit 1
fi

old_ifs=$IFS
new_ifs="
"

IFS=$new_ifs
read prev_line < $1
dir=""
max=0
max_line=""

for line in $(cat $1); do 	
	IFS=$old_ifs

	if [ "$(echo $line | cut -f1 -d' ')" = "total" ]; then
		
		if [ "$max_line" ]; then
			echo $dir
			echo $max_line
		fi

		max=0
		max_line=""
		dir=$prev_line
		continue
	elif ! [ "$line" ]; then
		dir=""
	fi

	if ! [ "$dir" ]; then
		continue
	elif [ "$(echo $line | cut -b$[$(echo $line | wc -c)-1])" = ":" ]; then
		prev_line=$line
		continue
	fi	
	
	if [ "$(echo $line | cut -b1)" = "-" ]; then
		if [ $(echo $line | cut -f5 -d' ') -gt $max ]; then
			max=$(echo $line | cut -f5 -d' ')
			max_line=$line
		fi
	fi
	
	IFS=$new_ifs
done
