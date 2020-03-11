#include "tiffio.h"
#include <stdio.h>
#include <stdlib.h>

void main()
{

/* This .tiff file is essentially a stack of 2D images - this code tells us how many images
 * are in the stack.
 */
    TIFF *tif = TIFFOpen("img100_1.tif", "r");
    if (tif) {
	int dircount = 0;
	do {
	    dircount++;
	} while (TIFFReadDirectory(tif));
	printf("%d directories in %s\n", dircount, "img100_1");
    }

    if (tif) {
	uint32 w, h;
	size_t npixels;
	uint32* raster;

	TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
	TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
        printf("TIFF width: %d\n", w);
        printf("TIFF height: %d\n", h);
	npixels = w * h;
	raster = (uint32*) _TIFFmalloc(npixels * sizeof (uint32));
	if (raster != NULL) {
	    if (TIFFReadRGBAImage(tif, w, h, raster, 0)) {
                // manipulate raster data here
	    }
	    _TIFFfree(raster);
	}
    }


    TIFFClose(tif);
    exit(0);
}
