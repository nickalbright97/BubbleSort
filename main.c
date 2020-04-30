#include "tiffio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32 width, height, depth, buffsize; // global variables

void printArray(char * array, uint32 width);
void printDistArray(char * array, uint32 width);
int getIndexArray(char * array, uint32 width, int x_i, int y_i, int z_i);
int getDistIndexArray(char * array, uint32 width, int x_i, int y_i, int z_i);
void setIndexArray(char * array, uint32 width, int x_i, int y_i, int z_i, int val);
char * find_dist(char *g, int w, int h, int z);
void makeCSVs(char * array);

void main()
{
   int dircount;

/* This .tiff file is essentially a stack of 2D images - this code tells us how many images
 * are in the stack.
 */
    TIFF *tif = TIFFOpen("images/img100_1.tif", "r");
    if (tif) {
	dircount = 0;
	do {
	    dircount++;
//          Enable line below to see directory info - very useful
//          TIFFPrintDirectory(tif, stdout, 0);
	} while (TIFFReadDirectory(tif));
	printf("%d directories in %s\n", dircount, "img100_1");
    }

    if (tif) {
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

        /*int count = 0;
	for (int x = 0; x < width; x++) {
	    for (int y = 0; y < height; y++) {
		for (int z = 0; z < depth; z++) {
		    int tmp = getDistIndexArray(distBuf, buffsize, x, y, z);
		    if (tmp == 127) count++;
		}
	    }
	}
	printf("count: %d: \n", count);*/

	free(buf);	
        free(distBuf);
	free(xyzr);
        TIFFClose(tif);
    }
    exit(0);
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

void makeCSVs(char * array) {
    FILE *fp;
    char index[10];

    for (int z_i = 0; z_i < depth; z_i++) {
        char *filename;
        snprintf(filename, 20, "CSVs/arr%d.csv", z_i);
        fp = fopen(filename, "w+");
        
        for (int i = 0; i < width*height; i++) {
            if (i % width == 0 && i != 0) { fprintf(fp, "\n"); }
            unsigned char c = (unsigned char) array[z_i*width*height + i];
            if ((i+1) % width == 0) {
                fprintf(fp, "%d", c);
            } else {
                fprintf(fp, "%d,", c);
            }
            /*
            for (int j = 0; j < 100; j++) {
                printf("%d\n",j);
              //  c = (unsigned char) array[z_i*w*h + y*w + x + 1];
                fprintf(fp, ",%d", 1);
            } 
            */
            
        }
        fclose(fp);
    }
}

void printArray(char * array, uint32 maxSize)
{
    uint32 i;
    
    for (i = 0; i < maxSize/279; i++)
    {
        // Have to convert to unsigned char or sign-extension will ruin high values
        unsigned char c = (unsigned char) array[i];
        printf("%d ", c);
        if (i % width == 0)
            printf("End of row %d", i / width - 1);
        if (i % 16 == 0)
            printf("\n"); 
    }
    printf("\n");

    /*    
    //  Use this to print a single pixel from each slice (offset is added to array index)
    for (i = 0; i < 279; i++) {
        unsigned char c = (unsigned char) array[i*512*512 + 512*5 + 486];
        printf("%d\n", c);
    }
    */
    

}

void printDistArray(char * array, uint32 maxSize)
{
    uint32 i;

    for (i = 0; i < maxSize*279; i++)
    {
     	// Have to convert to unsigned char or sign-extension will ruin high values
        char c = array[i];
        printf("%d ", c);
        if (i % width == 0)
            printf("End of row %d", i / width - 1);
        if (i % 16 == 0)
            printf("\n");
    }
    printf("\n");

    /*
    //  Use this to print a single pixel from each slice (offset is added to array index)
    for (i = 0; i < 279; i++) {
        char c = array[i*512*512 + 512*5 + 486];
        printf("%d\n", c);
    }
    */


}
