#include "tiffio.h"
#include <stdio.h>
#include <stdlib.h>

void printArray(char * array, uint32 width);

void readHeader() {
    // Read header of tiff file
    FILE *fp;
    unsigned char buffer[40];
    int i;
    char c;
    fp = fopen("img100_1.tif", "rb");
    fseek(fp, 0, SEEK_SET);
    for (i = 0; i < 40; i++) {
       c = fgetc(fp);
       buffer[i] = c;
       printf("Byte %d: %X\n", i, c);
    }
    fclose(fp);
}

void main() {

/* This .tiff file is essentially a stack of 2D images - this code tells us how many images
 * are in the stack.
 */
    TIFF *tif = TIFFOpen("img100_1.tif", "r");
    int dircount;
    if (tif) {
        dircount = 0;
        do {
            dircount++;
            // Enable line below to see directory info - very useful
            //TIFFPrintDirectory(tif, stdout, 0);
        } while (TIFFReadDirectory(tif));
        printf("%d directories in %s\n", dircount, "img100_1");
    }

    if (tif) {
        uint32 w, h;
        size_t npixels;
        unsigned char *buf = malloc(16 * 512 * 32); // One strip has 16 rows of 512
        uint32 row;
        uint32 config;

        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
        TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &config);
        printf("TIFF width: %d\n", w);
        printf("TIFF height: %d\n", h);
        printf("TIFF config: %d\n", config);
        npixels = w * h;
        
        int curDirectory = -1;

        for ( int i = 0; i < dircount; i++ ) {
            TIFFSetDirectory(tif, i);
            curDirectory = TIFFCurrentDirectory(tif);
            printf("Current Directory: %d\n", curDirectory);
                
            // Read directory 0
            for (int j = 0; j < 32; j++) { // 32 strips in slice
                    TIFFReadEncodedStrip(tif, j, &buf[512 * 16 * j], 16 * 512);
            }

            // printf("Current Directory: %d\n", curDirectory);
            printArray(buf, 512 * 16 * 32);
        }

            
        free(buf);
        TIFFClose(tif);
    }

    exit(0);

}

void printArray(char * array, uint32 width) {

    uint32 i;
    
    for (i = 0; i < width; i++) {
        printf("%d ", array[i] + 128);
        if (i % 512 == 0)
            printf("End of row %d", i / 512 - 1);
        if (i % 16 == 0)
            printf("\n"); 
    }
    printf("\n");
    
    /*  Use this to print a column of values (offset is added to array index)
    for (i = 0; i < 16 * 32; i++) {
        printf("%d\n", array[i*512 + 250] + 128);
    }
    */

}
