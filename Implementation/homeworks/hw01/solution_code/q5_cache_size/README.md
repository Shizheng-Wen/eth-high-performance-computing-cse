```
Follow the following instructions to compile, run and plot the results for Q5: Cache size and speed:

Q5a.
grep . /sys/devices/system/cpu/cpu0/cache/index*/*
# to obtain the sizes of L1, L2 and L3 cache of a computation node on Euler.
# you would have to get an interactive node of specific type on Euler first using:
# bsub -R "select[model==XeonGold_5118]" -W 00:30 -n 1 -Is bash
# or use directly the provided bash scipt:
./get_euler_cache_info.sh 

###################################################

Q5b, c, d.
# Execute and plot your implementation:
make run # (locally or on interactive node on Euler)
make submit # to run on compute node on Euler, while you are on a login node
# this will create a txt file results.txt with timings to plot

make plot : to plot performance plot 
```
