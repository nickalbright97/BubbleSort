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
