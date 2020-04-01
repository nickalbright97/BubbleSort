all : import main

import: import.c
	gcc -ltiff -std=c99 -o import import.c -I.
main: main.c
	gcc -ltiff -std=c99 -o main main.c -I.
