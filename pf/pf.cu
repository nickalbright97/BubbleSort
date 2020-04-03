/*
 * CS 470 Research Project
 * Team BubbleSort
 */

#include <stdio.h>
#include <stdlib.h>

// Custom timing macros
#include "timer.h"

// Controls debug output
//#define DEBUG

// Greyscale maximum (Set to 3 rather than 255 so there is a 33% chance it is 0)
#define GMAX 3

// Size of image in pixels
#define SIZE 512

// Greyscale vals for each pixel
int *gscales;

// Distance vals for each pixel
int *dist;

int total_pixels;

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
		int index = (SIZE * SIZE) * x + SIZE * y + z;
                gscales[index] = r;  // Assign random int to gscale index
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
		int index = (SIZE * SIZE) * x + SIZE * y + z;
                printf("%d ", gscales[index]);
            }
        }
    }
    printf("\n\nX val is each block");
    printf("\nY val is each row");
    printf("\nZ val is each column\n");
}


// Search for the nearest white pixel
__global__ void find_dist(int n, int *g, int *d)
{
    int index = blockIdx.x * blockDim.x + threadIdx.x; 
    int stride = blockDim.x * gridDim.x;
    for (int i = index; i < n; i += stride)
    {
        int x1 = i/(SIZE*SIZE);
        int y1 = (i%(SIZE*SIZE))/SIZE;
	int z1 = (i%(SIZE*SIZE))%SIZE;

        int index1 = (SIZE * SIZE) * x1 + SIZE * y1 + z1;
                
        if (g[index1] == 0)
        {
            d[index1] = 0;
        }
        else
        {
            int lowestDist = SIZE;
            for (int j = 0; j < n; j++) 
            {
                int x2 = j/(SIZE*SIZE);
                int y2 = (j%(SIZE*SIZE))/SIZE;
                int z2 = (j%(SIZE*SIZE))%SIZE;

	        int index2 = (SIZE * SIZE) * x2 + SIZE * y2 + z2;
                                
	        if (g[index2] == 0)
                {
                    int tmpDist = ((x2-x1)*(x2-x1)) + ((y2-y1)*(y2-y1)) + ((z2 - z1)*(z2-z1));
                    if (tmpDist < lowestDist)
                    {
                        lowestDist = tmpDist;
                    } 
                }
	    }

            d[index1] = lowestDist;
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
		int index = (SIZE * SIZE) * x + SIZE * y + z;
                printf("%d ", dist[index]);
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
    file = fopen("output.txt", "w+");

    total_pixels = SIZE*SIZE*SIZE;
    cudaMallocManaged(&gscales, (total_pixels * sizeof(int)));
    cudaMallocManaged(&dist, (total_pixels * sizeof(int))); 
    
    rand_gscales(); // Allocate random gscale values to array

    #ifdef DEBUG
    printf("\nDebug Output\n");
    debug_rand();
    #endif 

    int blockSize = 1024;
    int numBlocks = total_pixels / blockSize;

    START_TIMER(findDist);

    find_dist<<< numBlocks, blockSize >>>(total_pixels, gscales, dist);
    cudaDeviceSynchronize();

    STOP_TIMER(findDist);

    #ifdef DEBUG
    debug_dist();
    #endif

    fprintf(file, "DIST: %8.4fs\n", GET_TIMER(findDist));

    fclose(file);
    cudaFree(gscales);
    cudaFree(dist);
    return EXIT_SUCCESS;
}

