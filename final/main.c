#include "tiffio.h"
#include "index.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32 width, height, depth, buffsize; // global variables

int getIndexArray(char * array, uint32 width, int x_i, int y_i, int z_i);
int getDistIndexArray(char * array, uint32 width, int x_i, int y_i, int z_i);
void setIndexArray(char * array, uint32 width, int x_i, int y_i, int z_i, int val);

int main(int argc, char *argv[])
{
   int dircount;

    if (argc != 2) {
        printf("Not enough command line arguments.");
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
    fprintf(fp, "%d directories in %s\n", dircount, "img100_1");

    uint32 rows_in_strip = 16; // Obtained from reading TIFFPrintDirectory output
    uint32 npixels;
    uint32 row;
    uint32 config;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &config);
    depth = dircount;
    printf("TIFF width: %d\n", width);
    printf("TIFF height: %d\n", height);
    printf("TIFF config: %d\n", config);
    // Initialize buffer using w * h * z        
    unsigned char *buf = malloc(width * height * depth); // buffer to store all image pixels
    npixels = width * height;
    buffsize = npixels * depth;
    uint32 strips_in_slice = height / rows_in_strip; // 32 
    int curDirectory = -1;
    // Iterate through TIFF Stack, storing values in buffer
    for (int j = 0; j < depth; j++) {
        TIFFSetDirectory(tif, j);
        curDirectory = TIFFCurrentDirectory(tif);
        // printf("Current Directory: %d\n", curDirectory);

        // Read directory 0
        for (int i = 0; i < strips_in_slice; i++) { // 32 strips in slice
            TIFFReadEncodedStrip(tif, i, &buf[(j * npixels) + (i * rows_in_strip * width)],
                rows_in_strip * npixels);
        }
    }

    
    //makeCSVs(buf);
            
    // allocate memory for distance buffer
    char *distBuf = malloc(width * height * depth);
    distBuf = find_dist(buf, width, height, depth);
    
    // printDistArray(distBuf, 2);
    
    // printf("%d\n", indexArray(buf, buffsize, 4, 511, 0));

    // Make slices of array to find local maxima - this parameter can be adjusted
    int x_slice = width / 10;
    int y_slice = height / 10;
    int z_slice = depth / 10;
    printf("x: %d, y: %d, z: %d\n", x_slice, y_slice, z_slice);
    
    // initialize buffer for xyzr output
    int *xyzr = malloc(x_slice * y_slice * z_slice * 4);
    int bubCount = 0; // counter for bubbles
    int localx, localy, localz = 0;
    int localmax = 0;
    int dist = 0;
    
    // find local maximums
    int xi, yi, zi = 0; // slice indices
    int x, y, z = 0; // array indices
    while ((zi + 1 )* z_slice < depth) { // each while loop ensures slices dont outpace total array size
        while ((yi + 1) * y_slice < height) {
            while ((xi + 1) * x_slice < width) {
                localmax = 0;
                dist = 0;
                for (int z = zi * z_slice; z < (zi + 1) * z_slice; z++) { // each for loop iterates through each cell in each x,y,z slice
                    for (int y = yi * y_slice; y < (yi + 1) * y_slice; y++) {
                        for (int x = xi * x_slice; x < (xi + 1) * x_slice; x++) {
                            dist = getDistIndexArray(distBuf, buffsize, x, y, z);
                            if (dist > localmax) {
                                localmax = dist;
                                localx = x;
                                localy = y;
                                localz = z;
                            }
                        }
                    }
                }
                xyzr[bubCount*4] = localx; // add local max to output array
                xyzr[bubCount*4 + 1] = localy;
                xyzr[bubCount*4 + 2] = localz;
                xyzr[bubCount*4 + 3] = localmax;
                bubCount++;
                xi++;
            }
            xi = 0;
            yi++;
        }
        yi = 0;
        zi++;
    }
    printf("bubbles: \n");
    for (int i = 0; i < sizeof(xyzr); i++) {
        if (i % 4 == 0) {
            printf("\n");
        }
        printf("%d,", xyzr[i]);
    }
    printf("\n");
    free(buf);	
    free(distBuf);
    free(xyzr);
    TIFFClose(tif);
    return EXIT_SUCCESS;
}

// Index array given x, y, and z indices, return unsigned char
int getIndexArray(char * array, uint32 maxSize, int x_i, int y_i, int z_i)
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
