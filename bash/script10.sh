#!/bin/bash

for i in {1..80}; do

    sleep 1
    echo -en "\r  $i% ["
    j=0

    while [ $j -lt $i ]; do
        echo -n "#"
        j=$[j+1]
    done

    while [ $j -lt 80 ]; do
        echo -n " "
        j=$[j+1]
    done 

    echo -en "]"
done

echo ""