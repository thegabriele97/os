#!/bin/bash

if [ $(ping -c 4 google.com 2>/dev/null | grep "64 bytes from" | wc -l) -ge 1 ]; then
    echo "You are connected to internet"
else
    echo "You aren't connected to internet"
fi

exit 0