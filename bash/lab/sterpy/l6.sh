#!/bin/bash

old_IFS=$IFS

used=0
skip=0
IFS="
"
for process in $(ps au); do

    if [ $skip -eq 0 ]; then
        skip=1
        continue;
    fi

    IFS=$old_IFS    
    i=0
    for field in $process; do

        if [ $i -eq 5 ]; then
            used=$[used+field]
            echo $used
        fi

        i=$[i+1]
    done    

    IFS="
    "
done

echo "$[$[used*100]/3014212]%"