#include <stdio.h>
#include <stdlib.h>
#include "tiffio.h"
#include <string.h>
#include "queue.h"

int getDistIndexArray(char * array, uint32 width, int x_i, int y_i, int z_i);
int getIndexArray(unsigned char * array, uint32 width, int x_i, int y_i, int z_i);
void setIndexArray(char * array, uint32 width, int x_i, int y_i, int z_i, int val);
int whiteThreshold = 190; // defines white

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
