#!/bin/bash

if ! [ $# -eq 2 ]; then
	exit 1
fi

old_ifs=$IFS
new_ifs="
"

data=""
not_save=0
read month_year < $1

IFS=$new_ifs
for line in $(cat $1); do
	if [ $not_save -eq 0 ]; then
		not_save=1
		continue
	fi
	
	data="${data}\n$line"
done
IFS=$old_ifs

echo $month_year > $2
i=1
while [ $i -le 7 ]; do
	col=$(echo -e $data | cut -f$i -d' ')
	
	for e in $col; do
		echo -en "$e " >> $2
	done

	i=$[$i+1]
	echo "" >> $2
done
