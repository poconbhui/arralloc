Arralloc
========

Arralloc allocates an N dimensional array that can be accessed using
square braces.

It also allows the array do be dereferenced to provide
a flattened array of data where storage conventions follow regular C
conventions.

Finally, the entire array has been allocated with one
call to malloc, so it can be freed using one call to free.

Example Usage:

    int ***arr3d = arralloc(sizeof(int), 3, 6,7,8);

    /* Array accessible and settable using [i][j][k] */
    for(i=0; i<6; i++)
    for(j=0; j<7; j++)
    for(k=0; k<8; k++) {
        arr3d[i][j][k] = i*7*8 + j*8 + k;
    }

    /* Access a flattned array of the data */
    int *flat = &arr3d[0][0][0];

    for(i=0; i<6; i++)
    for(j=0; j<7; j++)
    for(k=0; k<8; k++) {
        /* Will print (i*7*8 + j*8 + k) */
        printf("%d\n", flat[i*7*8 + j*8 + k]);
    }

    /* Free the entire array plus pointer arrays */
    free(arr3d);
