#!/bin/bash

old_ifs=$IFS
new_ifs="
"

for file in $(ls | grep *.cal); do
	IFS=$new_ifs
	
	for date in $(cat $file); do
		day=$(echo $date | cut -f1 -d' ')
		month=$(echo $date | cut -f2 -d' ')
		year=$(echo $date | cut -f3 -d' ')

		name=$(ncal $month $year | grep -w $day | cut -b1,2)
		echo $date $name
	done

	IFS=$old_ifs
done
