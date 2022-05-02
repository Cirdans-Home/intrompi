#!/bin/bash
#SBATCH --job-name=helloworld
#SBATCH --nodes=1
#SBATCH --ntasks=6
#SBATCH --time=00:05:00
#SBATCH -o ./%x.%j.out
#SBATCH -e ./%x.%j.err
mpirun helloworld
