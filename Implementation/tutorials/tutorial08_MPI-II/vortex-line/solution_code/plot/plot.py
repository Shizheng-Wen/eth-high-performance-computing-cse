import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

def read_data():
    data = []
    for i in range(10):
        points = np.loadtxt("../config_0000%d.csv"%i, delimiter=',', skiprows=1)
        data.append(points[:, :2])
    for i in range(10, 25):
        points = np.loadtxt("../config_000%2d.csv"%i, delimiter=',', skiprows=1)
        data.append(points[:, :2])
    return np.array(data)

def init():
    particles.set_data([], [])
    return particles

def animate(i):
    global data
    particles.set_data(data[i, :, 0], data[i, :, 1])
    return particles

data = read_data()
fig, ax = plt.subplots()
xmin = np.min(data[:, :, 0]) - 0.2
xmax = np.max(data[:, :, 0]) + 0.2
ymin = np.min(data[:, :, 1]) - 0.2
ymax = np.max(data[:, :, 1]) + 0.2
plt.xlim(xmin, xmax)
plt.ylim(ymin, ymax)
# change the particle size
size = 1
particles, = ax.plot([], [], 'bo', ms=size)

ani = animation.FuncAnimation(fig, animate, frames=25, interval=10, init_func=init)
ani.save('particles.gif', fps=8)
#plt.show()
