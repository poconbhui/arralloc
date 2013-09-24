#ifndef ARRALLOC_H_
#define ARRALLOC_H_

#include <stdlib.h>
#include <stdarg.h>


/**
 * arralloc returns an N dimensional array, indexable by square brackets.
 * The whole thing has been allocated in the same call to malloc,
 * so a single call to free on the returned pointer should deallocate
 * the entire thing.
 *
 * Example usage:
 *  int***  arr3d = (int***)  arralloc(sizeof(int), 3, 7,8,9);
 *  int**** arr4d = (int****) arralloc(sizeof(int), 4, 7,8,9,10);
 *
 * This should return a 7x8x9 array, acting similarly to arr3d[7][8][9]
 * being indexable as arr3d[i][j][k].
 *
 * This can be freed using
 *  free(arr3d);
 *
 * Important to note is the data is stored in a flat array, so using
 *  int* flat = &arr3d[0][0][0]
 * will give you a pointer to the start of the flattened data.
 * The data is laid out in memory as you would expect C to lay it out.
 *
 * The routine varralloc is also provided, which expects
 * a va_list containing num_dims of ints representing the dimensions.
 */

void** arralloc(size_t pointer_size, int num_dims, ...);
void** varralloc(size_t sizeof_data, int num_dims, va_list dims_vl);


#endif  // ARRALLOC_H_
