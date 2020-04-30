all :  main

main: main.c distance.c
	gcc -ltiff -std=c99 -Wall -o main main.c distance.c -I.

par_main: par_main.c distance.c
	gcc -ltiff -std=c99 -fopenmp -Wall -o par_main par_main.c distance.c -I.
