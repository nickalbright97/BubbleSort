/*
 * CS 470 Research Project
 * Team BubbleSort
 */

#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

// Custom timing macros
#include "timer.h"

// Controls debug output
#define DEBUG

// Greyscale maximum (Set to 3 rather than 255 so there is a 33% chance it is 0)
#define GMAX 3

// Size of image in pixels
#define SIZE 4

// Greyscale vals for each pixel
int gscales[SIZE][SIZE][SIZE];

// Distance vals for each pixel
int dist[SIZE][SIZE][SIZE];

struct queue *worker_queue;

// Create random gscale vals for each pixel
void rand_gscales()
{
    for (int x = 0; x < SIZE; x++)
    {
     	for (int y = 0; y < SIZE; y++)
        {
            for (int z = 0; z < SIZE; z++)
            {
             	int r = rand() % GMAX; // Random int 0-2 (inclusive)
                gscales[x][y][z] = r;  // Assign random int to gscale index
            }
	}
    }
}

// Debug output random gscale values
void debug_rand()
{
    for (int i  = 0; i < SIZE * 2; i++)
    {
     	printf("-");
    }

    printf("\nRandom greyscale values:");
    for (int x = 0; x < SIZE; x++)
    {
     	printf("\n");
        for (int y = 0; y < SIZE; y++)
        {
            printf("\n");
            for (int z = 0; z < SIZE; z++)
            {
             	printf("%d ", gscales[x][y][z]);
            }
	}
    }
    printf("\n\nX val is each block");
    printf("\nY val is each row");
    printf("\nZ val is each column\n");
}


// Search for the nearest white pixel
void find_dist()
{
    for (int i = 1; isempty(worker_queue) != 1; i++)
    {
	int n = worker_queue->size;
        for (int j = 0; j < n; j++)
	{
            struct cell *c;
            c = dequeue(worker_queue);

            if (c->x-1 > -1 && dist[c->x-1][c->y][c->z] == -1)
	    {
	        dist[c->x-1][c->y][c->z] = i;
	        enqueue(worker_queue, create_element(c->x-1, c->y, c->z, dist[c->x-1][c->y][c->z]));
	    }
            if (c->x+1 < SIZE && dist[c->x+1][c->y][c->z] == -1)
            {
                dist[c->x+1][c->y][c->z] = i;
                enqueue(worker_queue, create_element(c->x+1, c->y, c->z, dist[c->x+1][c->y][c->z]));
	    }
            if (c->y-1 > -1 && dist[c->x][c->y-1][c->z] == -1)
            {
                dist[c->x][c->y-1][c->z] = i;
                enqueue(worker_queue, create_element(c->x, c->y-1, c->z, dist[c->x][c->y-1][c->z]));
	    }
            if (c->y+1 < SIZE && dist[c->x][c->y+1][c->z] == -1)
            {
                dist[c->x][c->y+1][c->z] = i;
                enqueue(worker_queue, create_element(c->x, c->y+1, c->z, dist[c->x][c->y+1][c->z]));
  	    }
            if (c->z-1 > -1 && dist[c->x][c->y][c->z-1] == -1)
            {
                dist[c->x][c->y][c->z-1] = i;
                enqueue(worker_queue, create_element(c->x, c->y, c->z-1, dist[c->x][c->y][c->z-1]));
	    }
            if (c->z+1 < SIZE && dist[c->x][c->y][c->z+1] == -1)
            {
                dist[c->x][c->y][c->z+1] = i;
                enqueue(worker_queue, create_element(c->x, c->y, c->z+1, dist[c->x][c->y][c->z+1]));
	    }

	    free(c);
        }
    }
}

// Debug output random gscale values
void debug_dist()
{
    for (int i  = 0; i < SIZE * 2; i++)
    {
     	printf("-");
    }

    printf("\nDistance values:");
    for (int x = 0; x < SIZE; x++)
    {
     	printf("\n");
        for (int y = 0; y < SIZE; y++)
        {
            printf("\n");
            for (int z = 0; z < SIZE; z++)
            {
             	printf("%d ", dist[x][y][z]);
            }
	}
    }
    printf("\n\nX val is each block");
    printf("\nY val is each row");
    printf("\nZ val is each column\n");
}

int main(int argc, char *argv[])
{
    FILE *file;

    file = fopen("output.txt", "w");

    rand_gscales(); // Allocate random gscale values to array

    #ifdef DEBUG
    printf("\nDebug Output\n");
    debug_rand();
    #endif

    worker_queue = (struct queue *)malloc(sizeof(struct queue));
    initqueue(worker_queue);

    for (int x = 0; x < SIZE; x++)
    {
	for (int y = 0; y < SIZE; y++)
	{
	    for (int z = 0; z < SIZE; z++)
	    {
		if (gscales[x][y][z] != 0)
		{
		    dist[x][y][z] = -1;
		}
	 	else
		{
		    struct cell *c = create_element(x, y, z, 0);
		    enqueue(worker_queue, c);
		}
	    }
	}
    }

    START_TIMER(findDist);

    find_dist();

    STOP_TIMER(findDist);

    #ifdef DEBUG
    debug_dist();
    #endif

    fprintf(file, "DIST: %8.4fs for %d thread\n", GET_TIMER(findDist), 1);

    free(worker_queue);
    fclose(file);
    return EXIT_SUCCESS;
}

