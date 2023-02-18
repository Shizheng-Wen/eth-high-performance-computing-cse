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


# plot temperature field
Nstep = 10000
Ndump = 1000

EXTENSIONS = ('.000')

counter=2
for root, dirs, files in os.walk('./'):
    for file in files:
        if file.endswith(EXTENSIONS):
           step=os.path.basename(file)
           data=np.genfromtxt(file)
           plt.figure(counter,figsize=(7,5))
           plt.plot(data[:,0],data[:,1],'o',color='crimson',markersize=12)
           plt.xlabel('x ',fontsize=20)
           plt.ylabel('T ',fontsize=20)
           plt.tick_params(axis='both',labelsize=17)
           plt.tight_layout()
           plt.savefig(step+'.png')
           counter+=1
