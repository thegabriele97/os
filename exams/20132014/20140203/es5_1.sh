#!/bin/bash

if ! [ $# -eq 4 ]; then
	exit 1
fi

old_IFS=$IFS
new_IFS="
"

if ! [ -e $3 ]; then
	mkdir $3
fi

set -x

for file1 in $(ls $1 | grep .txt); do
	file_name=$(echo $file1 | cut -f1 -d.)

	if [ $(cat $1/$file1 | wc -l) -gt $4 -a "$(ls $2/$file1)" != "" ]; then
		if [ $(cat $2/$file1 | wc -l) -gt $4  ]; then
			IFS=$new_IFS
			
			for line in $(cat $1/$file1); do
				
				if [ "$(cat $2/$file1 | grep -w $line)"  ]; then
					echo $line >> "$3/$file_name.eq"
				else
					echo $line >> "$3/$file_name.diff"
				fi
			done

			for line in $(cat $2/$file1); do
				
				if ! [ "$(cat $1/$file1 | grep -w $line)" ]; then
					echo $line >> "$3/$file_name.diff"
				fi
			done
			
			cat "$3/$file_name.eq" >> "$3/$file_name.cat"
			cat "$3/$file_name.diff" >> "$3/$file_name.cat"

			IFS=$old_IFS
		fi
	fi 

done
