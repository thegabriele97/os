#!/bin/bash

file=$1
file_back="${file}.xyx"
file_tmp="_$files"
old_IFS=$IFS

if ! [ -f $file ]; then
    exit 1
fi

cp $file $file_back
rm -f $file

IFS="
"
total_line=$(cat $file_back | wc -l)

for line in $(cat $file_back); do

    word_count=$(echo $line | wc -w)
    echo "$word_count:$total_line $line" >> $file_tmp

done

IFS=':'

sort -k 1,1 $file_tmp > $file
rm -f $file_tmp

IFS=old_IFS