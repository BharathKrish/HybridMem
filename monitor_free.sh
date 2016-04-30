#!/bin/bash
rm free_mem
while true
do
cat /proc/meminfo | grep "MemFree" >> free_mem
sleep $1
done
