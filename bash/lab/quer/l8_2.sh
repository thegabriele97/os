#!/bin/bash

file=$1

if ! [ $# -ge 1 ]; then
    echo -n "File name: "
    read file
fi

if ! [ -f $file ]; then
    echo "Unable to open $file."
    exit 1
fi

echo "File: $file"
echo "Your file contains $(cat $file | wc -l) lines"

max_len=0
max_line=""
for line in $(cat $file); do

    len=$(echo $line | wc -m)
    if [ $len -gt $max_len ]; then
        max_len=$len
        max_line=$line
    fi
done

echo "The longest line is $max_len characters long"

