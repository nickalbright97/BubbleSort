CC=gcc
CFLAGS=-g -Wall --std=c99 -Wno-unknown-pragmas -O3
NFLAGS=-ccbin $(CC) -g -O3
LIB=-lm
TARGETS=pf pf_serial pf_cuda

all: $(TARGETS)

pf: pf.c timer.h
	gcc -g -O2 --std=c99 -fopenmp -Wall -o pf pf.c

pf_serial: pf_serial.c timer.h
	gcc -g -O2 --std=c99 -Wall -o pf_serial pf_serial.c

pf_cuda: pf.cu
	nvcc $(NFLAGS) -o $@ $< $(LIB)

clean:
	rm -f pf pf_serial pf_cuda
