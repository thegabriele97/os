#!/bin/bash

echo -n "Directory: "
read dir
echo -n "Do you want to apply modifies on file systems? (y/n): "
read modify

modify_str=$modify
if [ $modify = "y" ]; then
    modify=0
else
    modify=1
fi

args=""
for arg in $*; do
    args=$args$arg
done

args_upper=$(echo $args | tr [:lower:] [:upper:])

for file in $(ls $dir); do
    
    if [ -f $dir/$file ]; then
        file_mod=$(echo $file | tr $args $args_upper)
        echo "$dir/$file_mod"

        if [ $modify -eq 0 ]; then
            mv $dir/$file $dir/$file_mod
        fi
    fi 
done

