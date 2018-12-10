#!/bin/bash

files=(lab01.txt lab02.txt lab03.txt)

for file in ${files[*]}; do

    wget -q http://fmgroup.polito.it/quer/teaching/so/laib/testi/$file

    l=$(cat < $file | wc -l)
    echo -e "# of lines in $file:\t$l"

    rm -f $file

done

exit 0