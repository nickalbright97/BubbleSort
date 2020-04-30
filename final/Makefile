default:  main par_main

main: main.c distance.c timer.h
	gcc -ltiff -g -std=c99 -Wall -o main main.c distance.c -I.

par_main: par_main.c distance.c timer.h
	gcc -ltiff -g -std=c99 -Wall -o par_main par_main.c -lpthread

clean:
	rm -f main par_main
