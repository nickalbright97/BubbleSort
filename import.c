#include "tiffio.h"
main()
{
    TIFF* tif = TIFFOpen("img100_1.tif", "r");
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);           // uint32 width;
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);        // uint32 height;
    TIFFClose(tif);
}
