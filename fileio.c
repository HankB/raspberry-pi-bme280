#include <stdio.h>
#include <errno.h>

#include "fileio.h"

/* open file to read values
    0 == success
    -1 == can't read three values
    errno == other file I/O problem
   close file if successfully opened.
*/
int get_vals(const char *filename, float *v1, float *v2, float *v3, unsigned int *count)
{
    FILE *f = fopen(filename, "r"); // open for read

    if (0 == f)
    {
        perror("Can't open file for reading");
        return errno;
    }

    int rc = fscanf(f, "%f%f%f%u", v1, v2, v3, count);
    fclose(f);

    if (rc != 4)
    {
        fprintf(stderr, "Can't read all values: %d\n", rc);
        return -1;
    }
    return 0;
}

/* open file to store values
    0 == success
    errno == file I/O problem
   close file if successfully opened.
*/
int put_vals(const char *filename, float v1, float v2, float v3, unsigned int count)
{
    FILE *f;
    f = fopen(filename, "w"); // read/write + create
    if (0 == f)
    {
        perror("Can't open file for writing");
        return errno;
    }
    int rc = fprintf(f, "%f %f %f %u\n", v1, v2, v3, count);
    perror("post fprintf");
    printf("put_vals() %d chars written\n", rc);
    rc = fclose(f);
    perror("post fclose");
    printf("fclose() %d, errno %d\n", rc, errno);
    return errno;
}
