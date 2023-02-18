#!/bin/bash
set -eu

tail -n 1 lsf.* | grep time | awk '{print $2; print NR; s+=$2*1.} END {print "avg time:", s/NR}'

