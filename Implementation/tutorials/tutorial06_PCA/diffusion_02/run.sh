#!/bin/bash
set -eu

export OMP_NUM_THREADS=12

for i in `seq 1 10`
do
    echo "Running case $i"
    bsub -n 12 -W 00:10 ./diffusion 100
done

