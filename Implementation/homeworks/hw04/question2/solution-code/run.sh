#!/bin/bash

o=out

lscpu

rm -f $o/*
mkdir -p $o

for i in 1 2 4 8 12 16 24 32 40 48; do

  echo "$i threads run"

  mpiexec -np $i main # add --oversubscribe if you have more than less than 48 cores
done
