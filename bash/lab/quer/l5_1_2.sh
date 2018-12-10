#!/bin/bash

process=$(ps aux | grep "root" | wc -l)

echo "Your system is executing $process processes as root"