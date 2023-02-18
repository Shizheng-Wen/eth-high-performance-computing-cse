#!/usr/bin/env python3

import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import re
import sys

path = 'particles.txt'
argv = sys.argv
if len(argv) > 1:
    path = argv[1]
data = np.loadtxt(path, skiprows=1)
data = np.atleast_2d(data)

with open(path) as f:
    header = f.readline()
header = header.split()

def select(block=None, N=None, colmaj=False, unopt=False):
    global data
    global header
    sel_block = block
    sel_N = N
    sel_colmaj = colmaj
    sel_unopt = unopt
    vblock = []
    vN = []
    vtime = []
    assert header[0] == 'N'
    for col in range(len(header)):
        for row in range(len(data)):
            h = header[col]
            if h == "N":
                continue
            elif h == "unopt":
                unopt = True
                block = None
            else:
                unopt = False
                block = int(re.findall(".._(\d*)", h)[0])
                assert h[:2] in ["bc", "br", "bs"], "unknown header: %r" % h
                colmaj = (h[1] == 'c')
            N = int(data[row][0] + 0.5)
            if sel_block is not None and block != sel_block:
                continue
            if sel_N is not None and N != sel_N:
                continue
            if colmaj != sel_colmaj:
                continue
            if unopt != sel_unopt:
                continue
            vblock.append(block)
            vN.append(N)
            vtime.append(data[row][col])
    return np.array(vblock), np.array(vN), np.array(vtime)


all_block, all_N, _ = select()

all_block = sorted(list(set(all_block)))
all_N = sorted(list(set(all_N)))

plt.figure(figsize=(10, 6))
N2color = dict()

for N in all_N:
    _, _, unopt_t = select(N=N, unopt=True)
    vb, vN, vt = select(N=N)
    line, = plt.plot(vb,
                     unopt_t/vt,
                     '--o',
                     markersize=8,
                     label="N={:}".format(N))

plt.ylim(0, None)
plt.xscale('log')
ax = plt.gca()
ax.set_xticks(all_block)
ax.get_xaxis().set_major_formatter(matplotlib.ticker.ScalarFormatter())
ax.tick_params(axis='x',
               which='minor',
               bottom=False,
               top=False,
               labelbottom=False)

plt.title('Cache Optimization for LJ Simulation')
plt.ylabel('Speed-Up')
plt.xlabel('Block Size')

plt.legend(loc=0, handlelength=4)
plt.grid(True)

plt.savefig('particles.pdf')
