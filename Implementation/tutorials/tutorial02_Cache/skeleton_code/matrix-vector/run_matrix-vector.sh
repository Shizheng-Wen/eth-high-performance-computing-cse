#!/bin/sh

#i being the matrix size in one dimension. So i = 300 -> matrix with dim(300,300)
for i in 250 500 750 1000 2000 3000  
do
    ./matrix-vector $i
done