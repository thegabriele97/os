#!/bin/bash

#checking for args
if ! [ $# -eq 3 ]; then
    echo "Missing arguments."
    exit 1
fi

log_file="log.txt"
tmp_out="_$3"

#checking if output file exists, then delete it
if [ -f $3 ]; then
    rm -f $3
fi

#checking if log file exists, then delete it
if [ -f $log_file ]; then
    rm -f $log_file
fi

#e is a entry given by ls -R (with useless row like dir/:)
for e in $(ls -R $1 2>> $log_file); do

    #path is created using find on "e". if "e" is a useless row like dir/:, 
    #it will give an error and will be ignored
    #if multiple files with same name exis, path will be an array
    path=$(find $1 -name "$e" 2>> $log_file)

    for right_entry in $path; do
        #if that entry exists
        if [ -f $right_entry ]; then
            #grep on that file
            #-nH will output file, number of line that match and matching row
            #output on $3
            grep -nH "$2" $right_entry 2>> $log_file >> $tmp_out
        fi
    done
done

#output will be now sorted on file name and row
sort -k 1,2 $tmp_out > $3

#removing tmp files
rm -f $log_file $tmp_out

exit 0