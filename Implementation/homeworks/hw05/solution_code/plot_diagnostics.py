import numpy as np
import matplotlib.pyplot as plt

def main():
    data = np.loadtxt('diagnostics.dat')

    print('data shape = ', data.shape)

    plt.plot(data[:,0], data[:,1])

    plt.xlabel('time')
    plt.ylabel('total mass')

    plt.savefig('diagnostics.png')
    print('plot saved in daignostics.png')
    
    plt.show()

if __name__ == "__main__":
    main()