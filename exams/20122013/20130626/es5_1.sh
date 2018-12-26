#!/bin/bash

if ! [ $# -eq 1 ]; then
	exit 1
fi

old_ifs=$IFS
new_ifs="
"

set -x

for entry in $(ls $1); do
	
	if [ -d "$1/$entry" ]; then
		./$0 "$1/$entry"
	elif [ -f "$1/$entry" ]; then
		file_ext=$(echo $entry | cut -f2 -d.)
		file_name=$(echo $entry | cut -f1 -d.)
		
		if [ ".$file_ext" = ".txt" ]; then
			IFS=$new_ifs
			cp "$1/$entry" "$1/${file_name}.dot"
			rm -f "$1/$entry"
			
			for line in $(cat "$1/${file_name}.dot"); do
				first_word=$(echo $line | cut -f1 -d " ")
				del=0

				for i in {0..9}; do
					if [ "$(echo $first_word | cut -b1)" = "$i" ]; then
						del=1						
						break;
					fi
				done

				if ! [ $del -eq 1 ]; then
					echo $line >> "$1/$entry"
				fi
			done
		fi 

		IFS=old_ifs
	fi

done
