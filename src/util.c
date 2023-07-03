#include "util.h"

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

/**
 * @brief Read a file into a buffer
 * @note It is upon the user to free the buffer
 * @param file_path
 * @return char*
 */
char *read_file(const char *file_path)
{
    FILE *fle = fopen(file_path, "r");
    if (!fle)
        return NULL;

    fseek(fle, 0, SEEK_END);
    long size = ftell(fle);
    fseek(fle, 0, SEEK_SET);
    char *buffer = (char *)malloc(sizeof(char) * size + 1);
    fread(buffer, 1, size, fle);
    buffer[size] = '\0';
    fclose(fle);
    return buffer;
}