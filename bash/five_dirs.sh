#!/bin/bash

if [ -e five ]; then
    exit 1
fi

mkdir five
mkdir five/dir{1..5}

for dir in five/dir{1..5}; do

    for file in {1..4}; do

        i=0
        while [ $i -lt $file ]; do

            j=0
            while [ $j -le $i ]; do
                echo -n $file >> "$dir/file$file"
                j=$[j+1]
            done

            if [ $[i+1] -lt $file ]; then
                echo "" >> "$dir/file$file"
            fi
        
            i=$[i+1]
        done
    done
done