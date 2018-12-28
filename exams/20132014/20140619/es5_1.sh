#!/bin/bash

if ! [ $# -eq 2 ]; then	
	exit 1
fi

old_ifs=$IFS
new_ifs="
"

IFS=$new_ifs
for entry in $(cat $1); do
	dir=$(echo $entry | cut -f1 -d" ")
	file=$(echo $entry | cut -f2 -d " ")

	if [ "$(cat $dir/$file | grep 'main')" ]; then
		echo -n "Do you want to copy $dir/$file? (y/n) "
		read
		
		if [ "$REPLY" = "y" ]; then
			cp $dir/$file $2/
		fi
	fi
done 
