/*
 * CS 470 Research Project
 * Team BubbleSort
 */

#include <stdio.h>
#include <stdlib.h>

// Custom timing macros
#include "timer.h"

// Controls debug output
// #define DEBUG

// Greyscale maximum (Set to 3 rather than 255 so there is a 33% chance it is 0)
#define GMAX 3

// Size of image in pixels
#define SIZE 20

// Euclidean Distance Algorithm helper function
int euclid_dist(int x1, int y1, int z1, int x2, int y2, int z2);

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
void find_dist(int x1, int y1, int z1)
{
    int lowestDist = SIZE;

    for (int x2 = 0; x2 < SIZE; x2++)
    {
	for (int y2 = 0; y2 < SIZE; y2++)
        {
            for (int z2 = 0; z2 < SIZE; z2++)
            {
             	if (gscales[x2][y2][z2] == 0)
                {
                    int tmpDist = euclid_dist(x1, y1, z1, x2, y2, z2);
                    if (tmpDist < lowestDist)
                    {
                     	lowestDist = tmpDist;
                    }
                }
            }
	}
    }
    dist[x1][y1][z1] = lowestDist;
}

// Finds the distance between two points
int euclid_dist(int x1, int y1, int z1, int x2, int y2, int z2)
{
    return ((x2-x1)*(x2-x1)) + ((y2-y1)*(y2-y1)) + ((z2 - z1)*(z2-z1));
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

    START_TIMER(findDist);

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

    STOP_TIMER(findDist);

    #ifdef DEBUG
    debug_dist();
    #endif

    fprintf(file, "DIST: %8.4fs for %d thread\n", GET_TIMER(findDist), 1);

    fclose(file);
    return EXIT_SUCCESS;
}

