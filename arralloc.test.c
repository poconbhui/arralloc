#include "arralloc.h"

#include <stdio.h>
#include <stdlib.h>


int main(void){

    /* our array pointers */
    long long *array_1d;
    long long **array_2d;
    long long ***array_3d;
    long long ****array_4d;

    /* some iterators */
    int i, j, k, l;

    /* The exit value of these tests */
    int exit_value = EXIT_SUCCESS;


    /*
     * test allocation of 1d array of size 500
     */

    fprintf(stderr, "Testing 1d version\n");

    /* Allocate the array */
    array_1d = (long long*) arralloc(sizeof(long long), 1, 500);

    /* Set data on the array */
    fprintf(stderr, "Testing setting\n");
    for(i=0; i<500; ++i){
        array_1d[i] = i;
    }

    /* Test retreiving data from the array */
    fprintf(stderr, "Testing retreiving\n");
    for(i=0; i<500; ++i) {
        if(array_1d[i] != i) {
            fprintf(
                stderr,
                "Incorrect value: array_1d[%d] is %d. Should be %d\n",
                i, (int)array_1d[i], i
            );

            exit_value = EXIT_FAILURE;
        }
    }

    /* Test freeing the array */
    fprintf(stderr, "Testing freeing\n");
    free(array_1d);

    /* Ok! */
    fprintf(stderr, "1d passed\n\n");


    /*
     * test allocation of 2d array of size 5x100
     */
    fprintf(stderr, "Testing 2d version\n");

    /* Allocate the array */
    array_2d = (long long**) arralloc(sizeof(long long), 2, 5,100);

    /* Set data on the array */
    fprintf(stderr, "Testing setting\n");
    for(i=0; i<5;  ++i)
    for(j=0; j<100; ++j) {
        array_2d[i][j] = i*100 + j;
    }

    /* Test retreiving data from the array */
    fprintf(stderr, "Testing retreiving\n");
    for(i=0; i<5;  ++i)
    for(j=0; j<100; ++j) {

        /* Ensure data retreivable with array syntax */
        if(array_2d[i][j] != i*100 + j) {
            fprintf(
                stderr,
                "Incorrect value: array_2d[%d][%d] is %d. Should be %d\n",
                i, j, (int)array_2d[i][j], i*100+j
            );

            exit_value = EXIT_FAILURE;
        }

        /* Ensure data is linear in memory */
        if((&array_2d[0][0])[i*100+j] != i*100 + j) {
            fprintf(
                stderr,
                "Incorrect value: (&array_2d[0][0])[%d] is %d. Should be %d\n",
                i*100+j, (int)(&array_2d[0][0])[i*100+j], i*100+j
            );

            exit_value = EXIT_FAILURE;
        }
    }

    /* Test freeing the array */
    fprintf(stderr, "Testing freeing\n");
    free(array_2d);

    /* Ok! */
    fprintf(stderr, "2d passed\n\n");


    /*
     * test allocation of 3d array of size 5x100x88
     */
    fprintf(stderr, "Testing 3d version\n");

    /* Allocate the array */
    array_3d = (long long***) arralloc(sizeof(long long), 3, 5,100,88);

    /* Set data on the array */
    fprintf(stderr, "Testing setting\n");
    for(i=0; i<5;  ++i)
    for(j=0; j<100; ++j)
    for(k=0; k<88; ++k) {
        array_3d[i][j][k] = i*100*88 + j*88 + k;
    }

    /* Test retreiving data from the array */
    fprintf(stderr, "Testing retreiving\n");
    for(i=0; i<5;  ++i)
    for(j=0; j<100; ++j)
    for(k=0; k<88; ++k) {

        /* Ensure data retreivable with array syntax */
        if(array_3d[i][j][k] != i*100*88 + j*88 + k) {
            fprintf(
                stderr,
                "Incorrect value: array_3d[%d][%d][%d] is %d. Should be %d\n",
                i, j, k, (int)array_3d[i][j][k], i*100*88 +j*88 + k
            );

            exit_value = EXIT_FAILURE;
        }

        /* Ensure data is linear in memory */
        if((&array_3d[0][0][0])[i*100*88 +j*88 + k] != i*100*88 + j*88 + k) {
            fprintf(
                stderr,
                "Incorrect value: (&array_3d[0][0][0])[%d] is %d. "
                "Should be %d\n",

                i*100*88 +j*88 + k,
                (int)(&array_3d[0][0][0])[i*100*88 +j*88 + k],
                i*100*88 +j*88 + k
            );

            exit_value = EXIT_FAILURE;
        }
    }

    /* Test freeing the array */
    fprintf(stderr, "Testing freeing\n");
    free(array_3d);

    /* Ok! */
    fprintf(stderr, "3d passed\n\n");


    /*
     * test allocation of 4d array of size 5x100x88x7
     */
    fprintf(stderr, "Testing 4d version\n");

    /* Allocate the array */
    array_4d = (long long****) arralloc(sizeof(long long), 4, 5,100,88,7);

    /* Set data on the array */
    fprintf(stderr, "Testing setting\n");
    for(i=0; i<5;  ++i)
    for(j=0; j<100; ++j)
    for(k=0; k<88; ++k)
    for(l=0; l<7; ++l) {
        array_4d[i][j][k][l] = i*100*88*7 + j*88*7 + k*7 + l;
    }

    /* Test retreiving data from the array */
    fprintf(stderr, "Testing retreiving\n");
    for(i=0; i<5;  ++i)
    for(j=0; j<100; ++j)
    for(k=0; k<88; ++k)
    for(l=0; l<7; ++l) {

        /* Ensure data retreivable with array syntax */
        if(array_4d[i][j][k][l] != i*100*88*7 + j*88*7 + k*7 + l) {
            fprintf(
                stderr,
                "Incorrect value: array_4d[%d][%d][%d][%d] is %d. "
                "Should be %d\n",

                i, j, k, l, (int)array_4d[i][j][k][l],
                i*100*88*7 +j*88*7 + k*7 + l
            );

            exit_value = EXIT_FAILURE;
        }

        /* Ensure data is linear in memory */
        if(
            (&array_4d[0][0][0][0])[i*100*88*7 +j*88*7 + k*7 + l]
            != i*100*88*7 + j*88*7 + k*7 + l
        ) {
            fprintf(
                stderr,
                "Incorrect value: (&array_4d[0][0][0][0])[%d] is %d. "
                "Should be %d\n",

                i*100*88*7 +j*88*7 + k*7 + l,
                (int)(&array_4d[0][0][0][0])[i*100*88*7 +j*88*7 + k*7 + l],
                i*100*88*7 +j*88*7 + k*7 + l
            );

            exit_value = EXIT_FAILURE;
        }
    }

    /* Test freeing the array */
    fprintf(stderr, "Testing freeing\n");
    free(array_4d);

    /* Ok! */
    fprintf(stderr, "4d passed\n\n");

    return exit_value;
}
