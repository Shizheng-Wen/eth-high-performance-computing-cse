import numpy as np
import matplotlib.pyplot as plt

def phi_r_samples(n, radius):
    phi = np.random.uniform(0, 2*np.pi, size=n)
    r = np.random.uniform(0, radius**2, size=n)

    r = np.sqrt(r)

    x = r*np.cos(phi)
    y = r*np.sin(phi)

    return x, y

def reject_samples(n, radius):
    x = np.random.uniform(-radius, radius, size=n)
    y = np.random.uniform(-radius, radius, size=n)

    
    x_ = x[x*x + y*y < radius**2]
    y_ = y[x*x + y*y < radius**2]

    return x_, y_

def main():
    n = 10000
    radius = 5

    x, y = phi_r_samples(n, radius)

    #x, y = reject_samples(n, radius)


    plt.plot(x, y, '.')

    plt.xlim(-radius, radius)
    plt.ylim(-radius, radius)

    
    ax = plt.gca()
    ax.add_patch(plt.Circle((0,0), radius, fill=False))

    ax.axis('equal')

    #plt.show()
    plt.savefig("circle_plot.png")


if __name__ == "__main__":
    main()