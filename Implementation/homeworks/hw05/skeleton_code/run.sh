#!/bin/bash

N=256

lscpu

rm -f out_$N
mkdir -p out_$N

for i in 1 12 24 36 48; do

  echo "$i threads run with $N"

  mpirun -n $i ./diffusion 1.0 2.0 $N # add --oversubscribe if you have more than less than 48 cores
done

cat out_$N/* >> all_$N.txt

rm -f out_$N