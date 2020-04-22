#include "tiffio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32 w, h, z, buffsize; // global variables

void printArray(char * array, uint32 width);
int getIndexArray(char * array, uint32 width, int x_i, int y_i, int z_i);
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

	TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
	TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
        TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &config);
        z = dircount;
        printf("TIFF width: %d\n", w);
        printf("TIFF height: %d\n", h);
        printf("TIFF config: %d\n", config);
        // Initialize buffer using w * h * z        
        unsigned char *buf = malloc(w * h * z); // buffer to store all image pixels
	npixels = w * h;
        buffsize = npixels * z;
        uint32 strips_in_slice = h / rows_in_strip; // 32
	
        int curDirectory = -1;
        // Iterate through TIFF Stack, storing values in buffer
        for (int j = 0; j < z; j++) {
	    TIFFSetDirectory(tif, j);
            curDirectory = TIFFCurrentDirectory(tif);
            // printf("Current Directory: %d\n", curDirectory);

            // Read directory 0
 	    for (int i = 0; i < strips_in_slice; i++) { // 32 strips in slice
                TIFFReadEncodedStrip(tif, i, &buf[(j * npixels) + (i * rows_in_strip * w)],
                    rows_in_strip * npixels);
            }
        }

        // makeCSVs(buf);
        
        
	// allocate memory for distance buffer
	unsigned char *distBuf = malloc(w * h * z);
        distBuf = find_dist(buf, w, h, z);

        printArray(distBuf, buffsize);
        // printf("%d\n", indexArray(buf, buffsize, 4, 511, 0));

	free(distBuf);	
        
        free(buf);
        TIFFClose(tif);
    }
    exit(0);
}

// Index array given x, y, and z indices, return unsigned char
int getIndexArray(char * array, uint32 width, int x_i, int y_i, int z_i)
{
    if (x_i * y_i * z_i > width) { printf("ERROR: index > buffer length\n"); exit(-1); }

    unsigned char c = (unsigned char) array[z_i*w*h + y_i * w + x_i + 1]; 
    return c; // casting unsigned char to int in return ensures positive val
}

// Index array given x, y, and z indices, return unsigned char
void setIndexArray(char * array, uint32 width, int x_i, int y_i, int z_i, int val)
{
    if (x_i * y_i * z_i > width) { printf("ERROR: index > buffer length\n"); exit(-1); }

    array[z_i*w*h + y_i * w + x_i + 1] = val;
}

void makeCSVs(char * array) {
    FILE *fp;
    char index[10];

    for (int z_i = 0; z_i < z; z_i++) {
        char *filename;
        snprintf(filename, 20, "CSVs/arr%d.csv", z_i);
        fp = fopen(filename, "w+");
        
        for (int i = 0; i < w*h; i++) {
            if (i % 512 == 0 && i != 0) { fprintf(fp, "\n"); }
            unsigned char c = (unsigned char) array[z_i*w*h + i];
            if ((i+1) % 512 == 0) {
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

void printArray(char * array, uint32 width)
{
    uint32 i;
    
    for (i = 0; i < width/279; i++)
    {
        // Have to convert to unsigned char or sign-extension will ruin high values
        unsigned char c = (unsigned char) array[i];
        printf("%d ", c);
        if (i % 512 == 0)
            printf("End of row %d", i / 512 - 1);
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
