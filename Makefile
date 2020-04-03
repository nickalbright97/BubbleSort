all :  main

main: main.c
	gcc -ltiff -std=c99 -o main main.c -I.
