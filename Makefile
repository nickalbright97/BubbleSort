all :  main par_main

main: main.c distance.c
	gcc -ltiff -std=c99 -o main main.c distance.c -I.

par_main: par_main.c distance.c
	gcc -ltiff -std=c99 -o par_main par_main.c distance.c -I.
