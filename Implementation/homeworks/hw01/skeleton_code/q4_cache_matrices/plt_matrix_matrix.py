from cProfile import label
import numpy as np
import matplotlib.pyplot as plt

def main():
    data_labels = np.loadtxt("matrix_matrix_times.txt", max_rows=1, dtype=str)
    data = np.loadtxt("matrix_matrix_times.txt", skiprows=1)

    blocksizes = np.array([2, 4, 8, 16, 32, 64, 128, 256, 512])

    colors = ['r', 'g', 'b']

    for n in range(3):
        N = int(data[n,0])
        #plot unopt
        plt.axhline(2*N*N*N/data[n,1], color=colors[n], label='unopt N = ' + str(N))

        #plot block row-wise
        plt.plot(blocksizes, 2*N*N*N/data[n,2:11], str(colors[n]) + 'o-', label='blocking row N = ' + str(N))
        
        #plt block col-wise
        plt.plot(blocksizes, 2*N*N*N/data[n,11:20], str(colors[n]) + 'x--', label='blocking col N = ' + str(N))

        #plot lapack
        plt.axhline(2*N*N*N/data[n,20], color=colors[n], linestyle=':', label='Lapack N = ' + str(N))

    plt.xlabel("blocking sizes [N]")
    plt.ylabel("flops per seconds [flops/s]")

    plt.xscale('log', basex=2)  #basex -> base if newer version of matplotlib
    plt.yscale('log', basey=10) #same here

    plt.xticks(blocksizes)
    plt.legend()
    plt.grid()
    plt.savefig("MMM_plot.png")
    plt.show()

if __name__ == "__main__":
    main()