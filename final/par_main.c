#include "tiffio.h"
#include "timer.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32 width, height, depth, buffsize; // global variables

int getIndexArray(unsigned char * array, uint32 width, int x_i, int y_i, int z_i);
int getDistIndexArray(char * array, uint32 width, int x_i, int y_i, int z_i);
void setIndexArray(char * array, uint32 width, int x_i, int y_i, int z_i, int val);
char *find_dist(unsigned char *g, int w, int h, int z);
int whiteThreshold = 190; // defines white

int main(int argc, char *argv[])
{
   START_TIMER(tiff);

   int dircount;

    if (argc != 2) {
        printf("Usage: ./par_main <image>\n");
        return EXIT_FAILURE;
    }

    /* This .tiff file is essentially a stack of 2D images - this code tells us how many images
     * are in the stack.
     */
    TIFF *tif = TIFFOpen(argv[1], "r");
    if (!tif) {
        printf("Error opening tiff file.");
        return EXIT_FAILURE;
    }

    FILE *fp;
    fp = fopen("output.txt", "w+");
    dircount = 0;
    do {
        dircount++;
//          Enable line below to see directory info - very useful
//          TIFFPrintDirectory(tif, stdout, 0);
    } while (TIFFReadDirectory(tif));
    fprintf(fp, "%d directories in %s\n", dircount, argv[1]);

    uint32 rows_in_strip = 16; // Obtained from reading TIFFPrintDirectory output
    uint32 npixels;
    uint32 config;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &config);
    depth = dircount;
    fprintf(fp, "TIFF width: %d\n", width);
    fprintf(fp, "TIFF height: %d\n", height);
    fprintf(fp, "TIFF config: %d\n", config);
    // Initialize buffer using w * h * z        
    unsigned char *buf = malloc(width * height * depth); // buffer to store all image pixels
    npixels = width * height;
    buffsize = npixels * depth;
    uint32 strips_in_slice = height / rows_in_strip; // 32 
    
    // Iterate through TIFF Stack, storing values in buffer
    for (int j = 0; j < depth; j++) {
        TIFFSetDirectory(tif, j);

        // Read directory 0
        for (int i = 0; i < strips_in_slice; i++) { // 32 strips in slice
            TIFFReadEncodedStrip(tif, i, &buf[(j * npixels) + (i * rows_in_strip * width)],
                rows_in_strip * npixels);
        }
    }
 
    STOP_TIMER(tiff);
    START_TIMER(dist);

    // allocate memory for distance buffer
    char *distBuf = malloc(width * height * depth);
    distBuf = find_dist(buf, width, height, depth);

    STOP_TIMER(dist);

    free(buf);	
    free(distBuf);
    TIFFClose(tif);

    printf("TIFF: %8.4fs\n", GET_TIMER(tiff));
    printf("DIST: %8.4fs\n", GET_TIMER(dist));

    return EXIT_SUCCESS;
}

// Index array given x, y, and z indices, return unsigned char
int getIndexArray(unsigned char * array, uint32 maxSize, int x_i, int y_i, int z_i)
{
    if (x_i * y_i * z_i > maxSize) { printf("ERROR: index > buffer length\n"); exit(-1); }

    unsigned char c = (unsigned char) array[z_i*width*height + y_i * width + x_i + 1];
    return c; // casting unsigned char to int in return ensures positive val
}

// Index array given x, y, and z indices, return unsigned char
int getDistIndexArray(char * array, uint32 maxSize, int x_i, int y_i, int z_i)
{
    if (x_i * y_i * z_i > maxSize) { printf("ERROR: index > buffer length\n"); exit(-1); }

    char c = array[z_i*width*height + y_i * width + x_i + 1];
    return c;
}

// Index array given x, y, and z indices, return unsigned char
void setIndexArray(char * array, uint32 maxSize, int x_i, int y_i, int z_i, int val)
{
    if (x_i * y_i * z_i > maxSize) { printf("ERROR: index > buffer length\n"); exit(-1); }

    array[z_i*width*height + y_i * width + x_i + 1] = val;
}

char* find_dist(unsigned char *g, int w, int h, int d)
{
    char *dist;
    struct queue *worker_queue;
    int width;

    worker_queue = (struct queue *)malloc(sizeof(struct queue));
    initqueue(worker_queue);
    width = w * h * d; 
    dist = malloc(width); 

    // Create distance array, -1 for non-white pixels and 0 for white pixels
    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            for (int z = 0; z < d; z++)
            {
                if (getIndexArray(g, width, x, y, z) < whiteThreshold)
                {
		    setIndexArray(dist, width, x, y, z, -1);
                }
                else
                {
                    struct cell *c = create_element(x, y, z, 0);
                    enqueue(worker_queue, c);
                }
            }
        }
    }

    // Work while the worker queue isn't empty
    for (int i = 1; isempty(worker_queue) != 1; i++)
    {
     	int n = worker_queue->size;
	// Iterate through all of the elements equal to n
        for (int j = 0; j < n; j++)
        {
            struct cell *c;
            c = dequeue(worker_queue);

	    // Check left of x, if c is a non-white pixel then set distance to i
            if (c->x-1 > -1 && getDistIndexArray(dist, width, c->x-1, c->y, c->z) == -1)
            {
		setIndexArray(dist, width, c->x-1, c->y, c->z, i);
                enqueue(worker_queue, create_element(c->x-1, c->y, c->z, getDistIndexArray(dist, width, c->x-1, c->y, c->z)));
            }

	    // Check right of x, if c is a non-white pixel then set distance to i
            if (c->x+1 < w && getDistIndexArray(dist, width, c->x+1, c->y, c->z) == -1)
            {
		setIndexArray(dist, width, c->x+1, c->y, c->z, i);
                enqueue(worker_queue, create_element(c->x+1, c->y, c->z, getDistIndexArray(dist, width, c->x+1, c->y, c->z)));
            }

	    // Check left of y, if c is a non-white pixel then set distance to i
            if (c->y-1 > -1 && getDistIndexArray(dist, width, c->x, c->y-1, c->z) == -1)
            {
  		setIndexArray(dist, width, c->x, c->y-1, c->z, i);
                enqueue(worker_queue, create_element(c->x, c->y-1, c->z, getDistIndexArray(dist, width, c->x, c->y-1, c->z)));
            }

	    // Check right of y, if c is a non-white pixel then set distance to i
            if (c->y+1 < h && getDistIndexArray(dist, width, c->x, c->y+1, c->z) == -1)
            {
		setIndexArray(dist, width, c->x, c->y+1, c->z, i);
                enqueue(worker_queue, create_element(c->x, c->y+1, c->z, getDistIndexArray(dist, width, c->x, c->y+1, c->z)));
            }

	    // Check left of z, if c is a non-white pixel then set distance to i
            if (c->z-1 > -1 && getDistIndexArray(dist, width, c->x, c->y, c->z-1) == -1)
            {
	 	setIndexArray(dist, width, c->x, c->y, c->z-1, i);
                enqueue(worker_queue, create_element(c->x, c->y, c->z-1, getDistIndexArray(dist, width, c->x, c->y, c->z-1)));
            }

	    // Check right of z, if c is a non-white pixel then set distance to i
            if (c->z+1 < d && getDistIndexArray(dist, width, c->x, c->y, c->z+1) == -1)
            {
		setIndexArray(dist, width, c->x, c->y, c->z+1, i);
                enqueue(worker_queue, create_element(c->x, c->y, c->z+1, getDistIndexArray(dist, width, c->x, c->y, c->z+1)));
            }

            free(c);
        }
    }

    free(worker_queue);

    return dist;
}
