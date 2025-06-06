#ifndef NU_MALLOC_H
#define NU_MALLOC_H

// Included for the `size_t` type.
#include <stddef.h>

// Declare function prototypes
void* nu_malloc(size_t);
void* nu_calloc(size_t, size_t);
void* nu_realloc(void*, size_t);
void nu_free(void*);

#endif
