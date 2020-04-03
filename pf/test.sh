#!/bin/bash                                                                                                                                                                                              $
#                                                                                                                                                                                                        $
#SBATCH --job-name=mcpi_omp                                                                                                                                                                              $
#SBATCH --nodes=1                                                                                                                                                                                        $
#SBATCH --ntasks=1                                                                                                                                                                                       $

make

./pf_serial

OMP_NUM_THREADS=2  ./pf
OMP_NUM_THREADS=4  ./pf
OMP_NUM_THREADS=8  ./pf
OMP_NUM_THREADS=16 ./pf
OMP_NUM_THREADS=32 ./pf
OMP_NUM_THREADS=64 ./pf
