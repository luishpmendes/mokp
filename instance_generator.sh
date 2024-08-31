#!/bin/bash

make instance_generator_exec

./bin/exec/instance_generator_exec --m 2 --n 1000 --max-x 4000 --min-p 10 --max-p 100 --min-w 10 --max-w 100 --seed 612158720 > instances/zlt_1000_2.txt

./bin/exec/instance_generator_exec --m 3 --n 1000 --max-x 4000 --min-p 10 --max-p 100 --min-w 10 --max-w 100 --seed 103469044 > instances/zlt_1000_3.txt

./bin/exec/instance_generator_exec --m 4 --n 1000 --max-x 4000 --min-p 10 --max-p 100 --min-w 10 --max-w 100 --seed 594038990 > instances/zlt_1000_4.txt

make clean
