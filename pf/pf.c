/*
 * CS 470 Research Project
 * Team BubbleSort
 */

#include <stdio.h>
#include <stdlib.h>

#ifdef _OPENMP
#include <omp.h>
#endif

// Custom timing macros
#include "timer.h"

// Controls debug output
// #define DEBUG

// Greyscale maximum (Set to 3 rather than 255 so there is a 33% chance it is 0)
#define GMAX 3

// Size of image in pixels
#define SIZE 20

// Greyscale vals for each pixel
int gscales[SIZE][SIZE][SIZE];

// Distance vals for each pixel
int dist[SIZE][SIZE][SIZE];

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
void find_dist(int x, int y, int z)
{
    /* TODO: 
     * Search in a radius for the nearest g=0 pixel
     * Suggestion for search: Use a counter variable, subtract every coordinate with it
     *     if still not found add every coordinate with it, if still not found increment counter
     * Example: x-1, y-1, z-1, x+1, y+1, z+1, x-2, y-2... until g=0 pixel found
     * Once you find it, plug in original and found pixel coordinates to euclid_dist
     */ 
}

// Finds the distance between two points
void euclid_dist(int x1, int y1, int z1, int x2, int y2, int z2)
{
   /* TODO:
    * Implement C Euclidean Distance Algorithm
    * Save the distance to the global dist array original pixel index (dist[x1][y1][z1] = dist)
    */
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
    rand_gscales(); // Allocate random gscale values to array

    #ifdef DEBUG
    printf("\nDebug Output\n");
    debug_rand();
    #endif

    // Check for nonzero pixels
    for (int x = 0; x < SIZE; x++)
    {
        for (int y = 0; y < SIZE; y++)
        {
            for (int z = 0; z < SIZE; z++)
            {
		// If the pixel is a nonzero gscale value
		// Find distance to closest pixel with zero gscale value
             	if (gscales[x][y][z] != 0)
		{
		    find_dist(x, y, z);
	        }
		// If the pixel is a zero gscale value, set dist value to 0
	 	else
		{
		    dist[x][y][z] = 0;
	        }
            }
	}
    }

    #ifdef DEBUG
    debug_dist();
    #endif

    return EXIT_SUCCESS;
}
