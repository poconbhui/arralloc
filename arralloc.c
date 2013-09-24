#include "arralloc.h"

#include <stdio.h>


/* If on an I860 align arrays to cache line boundaries */
#ifdef I860
#define MIN_ALIGN 32
#else
#define MIN_ALIGN 1
#endif


/**
 * get_alloc_size gets the overall size of memory we need to allocate
 * for all the pointer arrays plus the data.
 */
static size_t get_alloc_size(size_t sizeof_data, int num_dims, int* dims);


/**
 * set_pointers wires up our array indices so that arr[i][k]...[z] points
 * to (&arr[0]...[0])[i*dim_k*...*dim_z + ... + z].
 * That is, it does what we expect.
 */
static void set_pointers(
    size_t sizeof_data, void* arr, int num_dims, int* dims
);


/**
 * get_align_size finds the size of the word the data should be
 * aligned to.
 */
static size_t get_align_size(size_t sizeof_data);


/**
 * get_aligned_pointer returns a pointer that has been brought
 * up the the closest word boundary for a word of size align_size.
 */
static void* get_aligned_pointer(void* data, size_t sizeof_data);


void* arralloc(size_t sizeof_data, int num_dims, ...) {
    va_list vl;
    void *arr;

    va_start(vl, num_dims);
    arr = varralloc(sizeof_data, num_dims, vl);
    va_end(vl);

    return arr;
}


void* varralloc(size_t sizeof_data, int num_dims, va_list dims_vl) {
    int* dims;
    int alloc_size;
    void* arr;

    int i;


    /*
     * Check that requested data size is a multiple of a (void*).
     * The code only works for word objects.
     */
    if(sizeof_data % sizeof(void*) != 0) {
        return NULL;
    }

    if(NULL == (dims = (int*) malloc(sizeof(int)*num_dims))) {
        return NULL;
    }


    /* Set dims to the input dims */
    for(i=0; i<num_dims; ++i) {
        dims[i] = va_arg(dims_vl, int);
    }


    /* Find the total allocation size */
    alloc_size = get_alloc_size(sizeof_data, num_dims, dims);


    /* Allocate the array */
    if(NULL == (arr = (void*) malloc(alloc_size))) {
        return NULL;
    }


    set_pointers(sizeof_data, arr, num_dims, dims);


    free(dims);


    /* Return the array */
    return arr;
}


size_t get_align_size(size_t sizeof_data) {
   size_t align_size;

   if( sizeof_data > MIN_ALIGN ) {
   	align_size = sizeof_data;
   }
   else {
   	align_size = MIN_ALIGN;
   }
   while( (align_size % sizeof_data) || (align_size % MIN_ALIGN) ) {
   	align_size++;
   }

   return align_size;
}


void* get_aligned_pointer(void* data, size_t sizeof_data) {
    int align_size = get_align_size(sizeof_data);

    return (void*)(
        ( ((long)(data) + (align_size) - 1)/(align_size) )*(align_size)
    );
}


size_t get_alloc_size(size_t sizeof_data, int num_dims, int* dims) {
    size_t pointer_array_size;
    size_t data_array_size;

    int i;


    /**
     * We need to allocate a space dims[0]*...*dims[n-1] for our data.
     * The data is of size pointer_size.
     *
     * We must then allocate a space dims[0]*...*dims[n-2] for the
     * pointers to that data, and dims[0]*...*dims[n-3] for the pointers
     * to those pointers, and so on.
     * these pointers are of size sizeof(void*))
     */

    /* Find the total size of the pointer arrays */
    if(num_dims > 1) {

        /* Add first of pointer array size manually
         *
         * Necessary because for f(b) = a + a*b, there is no a
         * for which f(b) is the identity.
         */
        pointer_array_size = dims[0];

        /* Add each nested list of pointer arrays */
        for(i=1; i<num_dims-1; ++i) {
            pointer_array_size += pointer_array_size*dims[i];
        }

    } 
    /* For a 1d array, there are no pointer arrays */
    else {
        pointer_array_size = 0;
    }


    /* Find the data array size */
    data_array_size = 1;
    for(i=0; i<num_dims; ++i) {
        data_array_size *= dims[i];
    }

    return
        pointer_array_size*sizeof(void*)
        + data_array_size*sizeof_data
        + get_align_size(sizeof_data);
}


void set_pointers(size_t sizeof_data, void* arr, int num_dims, int* dims) {
    int offset=1;
    void*** p = (void***) arr;

    int d, i;


    /**
     * Importance is placed on data elements being laid out in memory
     * similarly to the native placements, so that accessing &arr[0]...[0]
     * should give you a pointer to a flat array of data.
     *
     * Since we expect our rightmost indices to be close in memory,
     * we set up our pointer lists is a similar manner.
     * This way, we may avoid even further delay when loading pointers
     * into memory from cache misses etc.
     *
     * There should be dims[0] values for arr[i] pointers,
     * dims[0]*dims[1] values for arr[*][j] * pointers etc
     * until dims[0]*...dims[n-1] values for arr[*][*]...[k]
     *
     * We consider each nesting arr[*][j] as an array of pointers where
     * arr[i] maps onto the array starting at arr[0][0].
     * As such, arr[i] defines an offset in arr[0][0], and arr[i][j]
     * should be equivalent to arr[0][i*dims[1]+j].
     *
     * Similarly, arr[i][j][k] should be equivalent to
     * arr[0][0][(i*dims[1]+j)*dims[0]*dims[1]+k].
     *
     *
     * From here, we can say that:
     *
     * Zeroth index:
     * arr[0] = &arr[dims[0]];
     * arr[i] = &arr[0][i*dims[1]]; 0<i<dims[0]
     *
     * First index:
     * arr[0][0] = &arr[0][dims[0]*dims[1]];
     * arr[0][i] = &arr[0][0][i*dims[2]]; 0<i<dims[0]*dims[1]
     *
     * Nth index:
     * arr[0]...[0] = &arr[0]...[dims[0]...dims[n]];
     * arr[0]...[i] = &arr[0]...[i*dims[n+1]]; 0<i<dims[0]*...dims[n]
     */


    /* Initialize p[0] for zeroth iteration. */
    p[0] = (void **) p;

    for(d=0; d<num_dims-2; ++d) {
        offset *= dims[d];

        /* Set p to start of next pointer list */
        /* p = &arr[0]...[0]                   */
        p = (void***) p[0];

        /* arr[0]...[0] = &arr[0]...[dims[0]*...*dims[n]] */
        p[0] = (void**) &p[offset];
        
        for(i=1; i<offset; ++i) {
            /* arr[0]...[i] = &arr[0]...[i*dims[n+1]] */
            p[i] = &p[0][i*dims[d+1]];
        }
    }

    /* Final iteration must take data size and alignment into account */
    d = num_dims-2;
    offset *= dims[d];

    p = (void***) p[0];

    p[0] = (void**) get_aligned_pointer(&p[offset], sizeof_data);
    
    for(i=1; i<offset; ++i) {
        /* Same as before, but moving along in memory in steps of sizeof_data */
        p[i] = &p[0][i*dims[d+1]*sizeof_data/sizeof(void*)];
    }
}
