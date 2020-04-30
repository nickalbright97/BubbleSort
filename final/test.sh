#!/bin/bash 
#
#SBATCH --job-name=mcpi_omp 
#SBATCH --nodes=1
#SBATCH --ntasks=1 

make

echo "SERIAL:" 
./main images/img100_1.tif

echo "PARALLEL:" 
OMP_NUM_THREADS=2  ./par_main images/img100_1.tif
OMP_NUM_THREADS=4  ./par_main images/img100_1.tif
OMP_NUM_THREADS=8  ./par_main images/img100_1.tif
OMP_NUM_THREADS=16 ./par_main images/img100_1.tif
OMP_NUM_THREADS=32 ./par_main images/img100_1.tif
OMP_NUM_THREADS=64 ./par_main images/img100_1.tif
