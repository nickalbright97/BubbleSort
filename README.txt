SOURCE CODE: https://github.com/nickalbright97/BubbleSort
DATA: tiff file in the image folder in the repo

idlCode/code.txt: 
	
	Program that our C code is based off of. Our goal was to convert this code to C code and parallelize it 
to get the run time (which would take several days for multiple images from a tiff file) to be faster.

images/img100_1.tif

	A sample image of a tiff file of what our code will process.

images/import.c

	Converts a tiff file into an array. In addition, it also does the reading-in of the file header for tiff files.
pf
	
	This folder contains all of the files for calculating pixel distance

pf/Makefile

	Compiles and cleans pf.c and pf_serial.c.

pf/output.txt

	Displays the runtime of a program.

pf/queue.h

	A queue for cells representing pixels of an image. Values in the nodes have x, y, and z coordinates as well as a value representing a pixel value in grayscale. The queue has associated enqueue and dequeue functions as well as functions to intialize a queue and determine if it is empty. 

pf/test.sh

	Test script that runs the pf_serial.c on one thread and pf.c on multiple threads.

pf/timer.h

	Timer to test the runtime of a program.

Makefile

	Compiles main.c

Main.c

	Converts a tiff file into an array. It is also able to output other meaningful information, such as the current directory being processed and the number of directories (images) in a tiff stack.



	
