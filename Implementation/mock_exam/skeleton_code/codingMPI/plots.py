import numpy as np
import matplotlib.pyplot as plt
import os

# plot diagnostics
diag = np.genfromtxt('diagnostics.dat')
plt.figure(1,figsize=(7,4))
plt.plot(diag[:,0],diag[:,1],'-o',markersize=12,linewidth=2,color='royalblue')
plt.xlabel('time ',fontsize=20)
plt.ylabel('integral ',fontsize=20)
plt.tick_params(axis='both',labelsize=17)
plt.tight_layout()
plt.savefig('diagnostics.png')

