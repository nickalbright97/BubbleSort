default: pf pf_serial

pf: pf.c timer.h
	gcc -g -O2 --std=c99 -fopenmp -Wall -o pf pf.c

pf_serial: pf_serial.c timer.h
	gcc -g -O2 --std=c99 -Wall -o pf_serial pf_serial.c

clean:
	rm -f pf pf_serial
