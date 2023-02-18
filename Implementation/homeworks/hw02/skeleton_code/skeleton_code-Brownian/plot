#!/bin/bash

gnuplot << EOF

set terminal pdf;
set output 'hist.pdf';
set key outside
set xlabel "x"
plot \
  "hist_0.dat" w l lw 3 ,\
  "hist_1.dat" w l lw 3 ,\

EOF
