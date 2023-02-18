import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np
from scipy.interpolate import griddata
import sys
import glob, os

def plot_snapshot(p):
  # grid size
  nx = 512
  ny = nx
  x1 = np.linspace(0., 1., nx)
  y1 = np.linspace(0., 1., ny)
  xg,yg = np.meshgrid(x1, y1)

  # read points
  x,y,u = np.loadtxt(p).T

  # resample to grid
  ug = griddata((x, y), u, (xg, yg), method='nearest')

  # init figure
  fig = plt.figure(figsize=(1.,1.), dpi=1, frameon=False)
  ax = plt.Axes(fig, [0., 0., 1., 1.])
  ax.set_axis_off()
  fig.add_axes(ax)

  # plot
  m = 1.
  u = np.clip(u, -m, m)
  ax.imshow(np.flipud(ug), vmin=-m, vmax=m, cmap=plt.get_cmap("coolwarm"))

  # save
  pb = os.path.splitext(p)[0]
  fo = "{:}.png".format(pb)
  fig.set_size_inches(1024,1024)
  fig.savefig(fo, dpi=1)
  matplotlib.pyplot.close()


for file in sorted(glob.glob("u*.dat")):
    print(file)
    plot_snapshot(file)
