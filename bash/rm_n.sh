#!/bin/bash

if ! [ $# -eq 2 ]; then
    exit 1
fi

for file in $(ls $1); do

    i=0
    for text in $(stat $1/$file); do

        if [ $i -lt 3 ]; then
            i=$[i+1]
            continue           
        fi

        if [ $text -gt $2 ]; then
            echo "Deleting $1/$file. size $text" >&2
            rm -f $1/$file
        fi

        break
    done
done