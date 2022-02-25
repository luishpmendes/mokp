#!/bin/bash

ns=(100 250 500 750)
ms=(2 3 4)

make instance_parser_exec

for n in ${ns[@]}
do
    for m in ${ms[@]}
    do
        ./bin/exec/instance_parser_exec < instances/knapsack.${n}.${m} > instances/zlt_${n}_${m}.txt
    done
done

make clean
