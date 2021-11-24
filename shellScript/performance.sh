#!/bin/sh

# Author : prasoon
# Script follows here:

module load gcc-9.2
echo "Generate Data File for Output"
DATA_FILE=$(touch performance.data)


echo "Experiment-1"
echo "Calculating Cache Performance"

echo "Compiling Memory Access Code"

g++ -fopenmp -Wall -o memory_access memory_access.cpp
sleep 2

echo "Calculating Performance values for different Threads"

for VARIABLE in 2 4 8 16 32 64
do
    perf stat -o out.txt -e cache-references,cache-misses ./memory_access ${VARIABLE} 0
    sleep 2
    filename='out_0.txt'
    OUTPUT_0=`awk '/cache-misses              #/ {print $4}' out.txt`
    # echo ${OUTPUT_0}

    perf stat -o out.txt -e cache-references,cache-misses ./memory_access ${VARIABLE} 1
    sleep 2
    filename='out_1.txt'
    OUTPUT_1=`awk '/cache-misses              #/ {print $4}' out.txt`
    # echo ${OUTPUT_1}

    echo "${OUTPUT_0} ${OUTPUT_1}" >> "performance.data"

done
