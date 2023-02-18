import matplotlib.pyplot as plt
import numpy as np
import glob

def main():
    data = np.loadtxt('yourdata.txt')   

    data = data[data[:, 0].argsort()]

    data = data.T

    data[1,:] = data[1,0]/data[1,:] # normalize the runs by the single core execution

    print(data)

    ideal_line = np.array([[data[0,0], data[0,-1]],[data[0,0], data[0,-1]]])
    plt.plot(data[0,:], data[1,:], '-xg', label="T(1)/T(n)")
    plt.plot(ideal_line[0,:], ideal_line[1,:], '--k', label="ideal line")
    plt.xlabel("# cores")
    plt.ylabel("weak scaling efficiency")
    plt.legend()
    plt.grid()
    plt.savefig("weak_scaling_efficiency.png")
    #plt.show()

if __name__ == "__main__":
    main()