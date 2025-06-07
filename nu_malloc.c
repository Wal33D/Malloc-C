
/* MIT License - see LICENSE file for details */
// <stddef.h> defines size_t; <string.h> provides memcpy and memset.
#include <stddef.h>
// Request optional bounds-checking interfaces (memset_s)
#define __STDC_WANT_LIB_EXT1__ 1
#include <string.h>
#ifdef _POSIX_VERSION
#include <sys/mman.h>
#else
#include <stdlib.h>
#endif
#include <stdint.h>
#include <stdalign.h>

#define NU_ALIGN alignof(max_align_t)
#define HEADER_SIZE ((sizeof(size_t) + NU_ALIGN - 1) & ~(NU_ALIGN - 1))
#include "nu_malloc.h"
#include <errno.h>

// Function to allocate memory
// Input: size_t size - the size of the memory block to be allocated
// Output: void* - a pointer to the allocated memory block
void *nu_malloc (size_t size) {
    /* Pointer to hold the start of the allocated memory */
    size_t* plen;
    /* Ensure we do not overflow when adding our header */
    if (size > SIZE_MAX - HEADER_SIZE) {
        errno = ENOMEM;
        return NULL;
    }
    size_t len = size + HEADER_SIZE;

    /* Allocate memory */
#ifdef _POSIX_VERSION
    plen = mmap(0, len, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
    if (plen == MAP_FAILED) {
        return NULL;
    }
#else
    plen = (size_t*)malloc(len);
    if (plen == NULL) {
        return NULL;
    }
#endif

    /* Store the length of allocated memory at the start */
    *plen = len;

    /* Return a pointer to the memory after the aligned header */
    return (void*)((char*)plen + HEADER_SIZE);
}

// Function to allocate and clear memory
// Input: size_t count - the number of elements to be allocated
// Input: size_t eltsize - the size of each element
// Output: void* - a pointer to the allocated and cleared memory block
// Returns NULL if multiplying count and eltsize would overflow SIZE_MAX
void *nu_calloc (size_t count, size_t eltsize) {
    /* Ensure multiplication does not overflow */
    if (eltsize != 0 && count > SIZE_MAX / eltsize)
        return NULL;

    /* Calculate the total size needed */
    size_t size = count * eltsize;
    /* Allocate memory */
    void *value = nu_malloc (size);
    /* Clear the memory */
    if (value != 0) {
#ifdef __STDC_LIB_EXT1__
        memset_s(value, size, 0, size);
#else
        memset(value, 0, size);
#endif
    }
    return value;
}

// Function to resize allocated memory
// Input: void* ptr - a pointer to the memory block to be resized
// Input: size_t size - the new size of the memory block
// Output: void* - a pointer to the resized memory block
void *nu_realloc (void *ptr, size_t size) {
    /* If the passed pointer is NULL, call nu_malloc */
    if (ptr == NULL) {
        return nu_malloc(size);
    }
    /* Pointer to the length of the original memory block */
    size_t* plen = (size_t*)((char*)ptr - HEADER_SIZE);
#ifdef _POSIX_VERSION
    /* Read the length of the original memory block */
    size_t len = *plen;
    size_t old_size = len - HEADER_SIZE;
#else
    (void)*plen;
#endif
#ifdef _POSIX_VERSION
    /* Allocate new memory */
    void* newptr = nu_malloc (size);
    if (newptr == NULL) {
        return NULL;
    }
    size_t min_size = (size < old_size) ? size : old_size;
    memcpy (newptr, ptr, min_size);
    nu_free (ptr);
    return newptr;
#else
    if (size > SIZE_MAX - HEADER_SIZE) {
        errno = ENOMEM;
        return NULL;
    }
    size_t new_len = size + HEADER_SIZE;
    size_t* newbase = realloc(plen, new_len);
    if (newbase == NULL) {
        return NULL;
    }
    *newbase = new_len;
    return (void*)((char*)newbase + HEADER_SIZE);
#endif
}

// Function to free allocated memory
// Input: void* ptr - a pointer to the memory block to be freed
void nu_free (void *ptr) {
    /* If the pointer is NULL there is nothing to free */
    if (ptr == NULL)
        return;

    /* Pointer to the length of the memory block */
    size_t* plen = (size_t*)((char*)ptr - HEADER_SIZE);
    /* Read the length of the memory block */
#ifdef _POSIX_VERSION
    size_t len = *plen;
    /* Free the memory using munmap */
    munmap((void*)plen, len);
#else
    (void)*plen; /* suppress unused-variable warning */
    /* Free the memory using the C library */
    free((void*)plen);
#endif
}
