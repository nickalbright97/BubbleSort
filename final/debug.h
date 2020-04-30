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
