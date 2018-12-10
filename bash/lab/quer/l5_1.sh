#!/bin/bash

dir=$HOME

if [ $# -eq 1 ]; then
    dir=$1
fi

#printing details
echo "Directory: $dir"
echo ""

count_file=0
count_dir=0
for entry in $(ls $dir); do

    path="$dir/$entry"

    if [ -f $path ]; then
        count_file=$[$count_file+1]
    elif [ -d $path ]; then
        count_dir=$[$count_dir+1]
    fi

done

#printing results
echo -e "Number of Files:\t$count_file"
echo -e "Number of Directory:\t$count_dir"

exit 0