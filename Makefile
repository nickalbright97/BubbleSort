all :  main

main: main.c distance.c
	gcc -ltiff -std=c99 -o main main.c distance.c -I.
