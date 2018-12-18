#!/bin/bash

if ! [ $# -eq 2 ]; then
    exit 1
fi

set -c

n=0
while [ "$(ps -el | grep -w $1)" ]; do

    i=0;
    for val in $(ps -el | grep $1); do

        if [ $i -eq 0 -o $i -eq 2 ]; then
            i=$[i+1]
            continue
        fi

        if [ $i -eq 1 -a $val != "Z" ]; then
            echo "Process $1 isn't a Zombie anymore"
            exit 0
        elif [ $i -eq 3 ]; then
            pid=$val
            break
        fi

        i=$[i+1]
    done

    if [ $n -eq 5 ]; then
        kill $pid
        exit 0
    fi

    n=$[n+1]
    sleep $2
done 

echo "Process $1 doesn't exists anymore in your system"
exit 0