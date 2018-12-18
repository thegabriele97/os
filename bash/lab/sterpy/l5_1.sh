#!/bin/bash

echo "ciao"
read -t 1 -n 1

if [ "$REPLY" = "s" ]; then
    echo ""
    exit 0
fi

./l5_2.sh