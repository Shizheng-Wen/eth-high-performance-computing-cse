# Principal Component Analysis

## Euler

This code is highly recommended to run on euler, due to the Lapack library being easily accessible on Euler.

If you are by default on the new software stack, first load the old stack
```
lmod2env
```
The load the modules
```
module load gcc
module load mkl
module load python
```


## Compilation

```
make
```


## Running

When trying to run submit the code with either one of these commands:
```
bsub -n 1 -R "select[model==XeonGold_6150]" -W 01:00 -Is bash
```
This will put you direclty inside the working node for 1 hour. This is usefull for directly debugging.
But it will take some time to get direct access to the node.
Inside the node, compile and type `./main` to run it.

OR
```
bsub -n 1 -R "select[model==XeonGold_6150]" -W 01:00 ./main
```
This method will directly put the code on a queue of the CPU model Xeon Gold 6150. It is very important, that
we run the code on an intel processor, otherwise it will not run lapack functions. 

