#!/bin/bash 
#
#SBATCH --job-name=mcpi_omp 
#SBATCH --nodes=1
#SBATCH --ntasks=1 

make

./main

OMP_NUM_THREADS=2  ./par_main
OMP_NUM_THREADS=4  ./par_main
OMP_NUM_THREADS=8  ./par_main
OMP_NUM_THREADS=16 ./par_main
OMP_NUM_THREADS=32 ./par_main
OMP_NUM_THREADS=64 ./par_main
