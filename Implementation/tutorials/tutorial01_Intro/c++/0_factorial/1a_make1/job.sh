#!/bin/bash -l

module load gcc
module list

#SBATCH -n 1
#SBATCH --time=0:10:00
#SBATCH --job-name=factorial
#SBATCH --mem-per-cpu=512
#SBATCH --output=file.out
#SBATCH --error=file.err
#SBATCH --open-mode=truncate

./factorial 10
