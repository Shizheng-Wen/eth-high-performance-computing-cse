#!/usr/bin/env python3

import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import sys
import glob

path = './states/'
argv = sys.argv
if len(argv) > 1:
    path = argv[1]

N = 100
if len(argv) > 2:
    N = int(argv[2])

# Get all files
stateFiles = glob.glob('{}state_*.txt'.format(path)) 
stateFiles.sort()

N = min(N, len(stateFiles))
print("Plotting first {} states from path {}".format(N, path))

# Read data into list
allData = []
for file in stateFiles[:N]:
    data = None
    with open(file) as f:
        data = np.array([np.array([float(x) for x in line.split()]) for line in f])
        allData.append(data)

# Plot data
for idx, points in enumerate(allData):
    fig = plt.figure()
    ax = fig.add_subplot(1,1,1,projection='3d')
    ax.axes.set_xlim3d(left=0.0, right=9.) 
    ax.axes.set_ylim3d(bottom=0., top=9.) 
    ax.axes.set_zlim3d(bottom=0., top=9.) 
    
    # Insert points into figure
    for point in points:
        x, y, z = point
        ax.scatter(x,y,z)

    print("Figure {}/{} created".format(idx, N))

    plt.savefig('figures/test{:06}.png'.format(idx))
    plt.close()

    

