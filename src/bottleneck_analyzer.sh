#!/bin/sh

# Author : Prasoon
# Script follows here:

module load gcc-9.2
echo "Generate performance counters..."
DATA_FILE=$(touch performance.data)

name=$(echo $1 | rev | cut -c5- | rev)

g++ -fopenmp -Wall -o ${name} $1
#g++ -pg -no-pie -fno-builtin -fopenmp -Wall -o ${name} $1

start=1
end=$#

echo "Generating program arguments..."
for (( c=$start; c<=$end; c++ ))
do
    if [[ $c -eq 2 ]]
    then
    args+=" $2"
    fi
    if [[ $c -eq 3 ]]
    then
    args+=" $3"
    fi
    if [[ $c -eq 4 ]]
    then
    args+=" $4"
    fi
    if [[ $c -eq 5 ]]
    then
    args+=" $5"
    fi
    if [[ $c -eq 6 ]]
    then
    args+=" $6"
    fi
done

# echo $args

# ==================================================================================================================
# ==================================================================================================================

echo "Generating memory access pattern counters..."

perf stat -o out.txt -e cache-references,cache-misses,L1-dcache-load-misses,L1-dcache-loads ./$name $args  >/dev/null
filename='out.txt'
OUTPUT=`awk '/cache-misses              #/ {print $4}' out.txt`
BENCHMARK=23.0

if (( $(echo "$OUTPUT > $BENCHMARK" |bc -l) ));
then
    FinalOutput+=$'Possible bad memory access patterns.\n'
fi

# ==================================================================================================================
# ==================================================================================================================

echo "Generating false sharing counters"

filename='out.txt'
OUTPUT=`awk '/L1-dcache-load-misses     #/ {print substr( $4, 1, length($4)-1 )}' out.txt`
BENCHMARK=0.9

if (( $(echo "$OUTPUT > $BENCHMARK" |bc -l) ));
then
    FinalOutput+=$'Possible false sharing.\n'
fi

# ==================================================================================================================
# ==================================================================================================================

echo "Generating omp construct overhead counters..."

operf ./$name $args >/dev/null
opreport --demangle=smart --symbols > out_2.txt 2>&1
filename='out_2.txt'
OUTPUT=`awk '{ if($3 ~ /libgomp/) sum += $2 } END { print sum }' out_2.txt`
BENCHMARK=8.0

if (( $(echo "$OUTPUT > $BENCHMARK" |bc -l) ));
then
    FinalOutput+=$'Possible overheads due to OMP constructs.\n'
fi

# ==================================================================================================================
# ==================================================================================================================

echo "Generating critical region counters..."

filename='out_2.txt'
OUTPUT=`awk '{ if($4 ~ /gomp_mutex_lock_slow/) sum += $2 } END { if(!sum) print "0" ; else print sum }' out_2.txt`
BENCHMARK=45

if (( $(echo "$OUTPUT > $BENCHMARK" |bc -l) ));
then
    FinalOutput+=$'Possible overheads due to long/numerous critical regions.\n'
fi

# ==================================================================================================================
# ==================================================================================================================

echo "Generating load balance counters..."

operf -t ./$name $args >/dev/null
opreport --demangle=smart --symbols > out_4.txt
init=0
d=()
count=0
while IFS= read -r line
do
    if [[ $init -eq 0 ]]; then
        if [[ $(echo $line | grep 'samples') ]]; then
            init=1
            count=$(echo $line | awk '{print NF/2 - 2}')
        fi
    else
        index=0
        for i in $line
        do
            if [[ $index -lt $count*2 ]]; then
                if [[ $(expr $index % 2) -eq 0 ]]; then
                    [[ -z ${d[$index]} ]] && d[$index]=0
                    d[$index]=$(expr ${d[$index]}  + $i)
                fi
                index=$(expr $index + 1)
            else
                index=0
                break;
            fi
        done
    fi
done < out_4.txt

max=${d[0]}
min=${d[0]}

for j in "${d[@]}"; do
    (( j > max )) && max=$j
    (( j < min )) && min=$j
done

s=`echo "$max - $min" | bc`
OUTPUT=`echo "scale=4; $s/$max" | bc`
BENCHMARK=0.5

if (($(echo "$OUTPUT > $BENCHMARK" |bc -l)));
then
    FinalOutput+=$'Possible load imbalance.\n'
fi

echo "-------------------------------------------------------"
echo "-------------------Bottleneck Summary------------------"
echo "-------------------------------------------------------"

echo ""
echo ""
echo "$FinalOutput"
echo ""

echo "-------------------------------------------------------"
echo "-------------------------------------------------------"
echo "-------------------------------------------------------"