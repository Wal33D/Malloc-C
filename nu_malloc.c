// Included for the `size_t` type.
#include <string.h>
#ifdef _POSIX_VERSION
#include <sys/mman.h>
#else
#include <stdlib.h>
#endif
#include <stdint.h>
#include "nu_malloc.h"
#include <errno.h>

// Function to allocate memory
// Input: size_t size - the size of the memory block to be allocated
// Output: void* - a pointer to the allocated memory block
void *nu_malloc (size_t size) {
    /* Pointer to hold the length of the allocated memory */
    size_t* plen;
    /*
    Add sizeof(size_t) for holding length
    */
    if (size > SIZE_MAX - sizeof(size_t)) {
        errno = ENOMEM;
        return NULL;
    }
    size_t len = size + sizeof(size_t);

    /* Allocate memory */
#ifdef _POSIX_VERSION
    plen = mmap(0, len, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
    if (plen == MAP_FAILED) {
        return NULL; /* errno from mmap is preserved */
    }
#else
    plen = (size_t*)malloc(len);
    if (plen == NULL) {
        return NULL;
    }
#endif

    /* Store the length of allocated memory in the first sizeof(size_t) bytes */
    *plen = len;                   

    /* Return a pointer to the memory after the length variable */
    return (void*)(&plen[1]);      
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
    if (value != 0)
        memset (value, 0, size);
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
    size_t* plen = (size_t*)ptr;
    /* Decrement the pointer to reach the top of the memory block */
    plen--;
#ifdef _POSIX_VERSION
    /* Read the length of the original memory block */
    size_t len = *plen;
    size_t old_size = len - sizeof(size_t);
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
    if (size > SIZE_MAX - sizeof(size_t)) {
        errno = ENOMEM;
        return NULL;
    }
    size_t new_len = size + sizeof(size_t);
    size_t* newbase = realloc(plen, new_len);
    if (newbase == NULL) {
        return NULL;
    }
    *newbase = new_len;
    return (void*)(&newbase[1]);
#endif
}

// Function to free allocated memory
// Input: void* ptr - a pointer to the memory block to be freed
void nu_free (void *ptr) {
    /* If the pointer is NULL there is nothing to free */
    if (ptr == NULL)
        return;

    /* Pointer to the length of the memory block */
    size_t* plen = (size_t*)ptr;
    /* Decrement the pointer to reach the top of the memory block */
    plen--;
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
