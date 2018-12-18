#!/bin/bash

file=$1

if ! [ $# -eq 1 ]; then
    echo -n "File name: "
    read file
fi

touch log
i=0

for word in $(cat $file); do

    if ! [ "$(cat log | grep -w $word)" ]; then
        echo "$word $i" >> log
        occ[$i]=0
        i=$[i+1]
    fi

    j=0
    for e in $(cat log | grep -w $word); do
        if [ $j -eq 0 ]; then
            j=1
            continue
        fi

        index=$e
        break
    done

    occ[$index]=$[${occ[$index]}+1]
done

j=0
for e in $(cat log); do

    if [ $j -eq 0 ]; then
        name=$e
        j=1
        continue
    fi

    echo $name ${occ[$e]}
    j=0
done

rm -f log
exit 0