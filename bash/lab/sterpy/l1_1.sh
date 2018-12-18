#!/bin/bash

if ! [ $# -eq 2 ]; then
    exit 1
fi

x=$1
u=$2

while [ $(echo $USER | grep "$u") ]; do
    echo x
    sleep $x
done

echo "User $u logged out"
exit 0