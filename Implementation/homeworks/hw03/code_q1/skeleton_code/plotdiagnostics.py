import matplotlib.pyplot as plt
import pandas
import numpy as np

if __name__ == "__main__":

    df = pandas.read_csv('diagnostics.dat', sep=' ')

    # Plot Concentration
    plt.figure()
    plt.plot(df['t'], df['concentration'])
    plt.title("Total Concentration")
    plt.xlabel("Time")
    plt.ylabel("Concentration")
    plt.savefig("concentration.png")

    
    # Plot Concentration Buckets (optional)
    t = [0.0, 0.25, 0.5]
    
    plt.figure()
    fig, ax = plt.subplots(1,len(t))
    fig.suptitle('Concentration Distribution')
    for idx in range(len(t)):
    
        row = df.iloc[(df['t']-t[idx]).abs().argsort()[0]]
        row = row.drop(['t', 'concentration']).to_numpy()
        ax[idx].bar(range(len(row)), row)
        ax[idx].set_title('t = {}'.format(t[idx]))
        ax[idx].set(xlabel='Bins')
        ax[idx].set_ylim(ax[0].get_ylim())
        ax[idx].label_outer()


    ax[0].set(ylabel='Count')
    plt.savefig("histograms.png")
