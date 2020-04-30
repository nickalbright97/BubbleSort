SOURCE CODE: https://github.com/nickalbright97/BubbleSort
DATA: tiff file in the image folder in the repo

final/images/img100-1.tif

	A sample image of a tiff file of what our code will process.

final/Makefile
	
	Compiles and cleans main.c, par_main.c, and distance.c

final/distance.c

	Calculates the distance between pixels

final/main.c

	Converts a tiff file into an array. It is also able to output other meaningful information, such as the current 		directory being processed and the number of directories (images) in a tiff stack.

final/output.txt

	Contains the number of directories and information of the height, width, and config.

final/par_main.c

	Parallel version of main.c. Parallelized using OpenMP

final/queue.h

	A queue for cells representing pixels of an image. Values in the nodes have x, y, and z coordinates as well as a 	value representing a pixel value in grayscale. The queue has associated enqueue and dequeue functions as well as 	functions to intialize a queue and determine if it is empty.

final/test.sh

	 Test script that runs the main.c on one thread and par_main.c on multiple threads.

final/timer.h

	Timer to test the runtime of a program.



	
