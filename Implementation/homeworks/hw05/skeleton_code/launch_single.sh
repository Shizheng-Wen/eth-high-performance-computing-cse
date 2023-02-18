#!/bin/bash

#SBATCH -n 48
#SBATCH --time=0:05:00
#SBATCH --job-name=mpi_run
#SBATCH --mem-per-cpu=512
#SBATCH --output=slurm_output.txt
#SBATCH --error=slurm_error.txt
#SBATCH --open-mode=truncate

mpirun -n 48 ./diffusion 1.0 2.0 256
