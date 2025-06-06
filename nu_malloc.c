// Included for the `size_t` type.
#include <string.h>
#include <sys/mman.h>
#include <stdbool.h>
#include "nu_malloc.h"

// Function to allocate memory
// Input: size_t size - the size of the memory block to be allocated
// Output: void* - a pointer to the allocated memory block
void *nu_malloc (size_t size) {
    /* Pointer to hold the length of the allocated memory */
    size_t* plen;
    /* 
    Add sizeof(size_t) for holding length
    */
    size_t len = size + sizeof(size_t);

    /* Allocate memory using mmap */
    plen = mmap(0, len, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
    /* Check if mmap failed */
    if (plen == MAP_FAILED) {
        return NULL;
    }

    /* Store the length of allocated memory in the first 4 bytes */
    *plen = len;                   

    /* Return a pointer to the memory after the length variable */
    return (void*)(&plen[1]);      
}

// Function to allocate and clear memory
// Input: size_t count - the number of elements to be allocated
// Input: size_t eltsize - the size of each element
// Output: void* - a pointer to the allocated and cleared memory block
void *nu_calloc (size_t count, size_t eltsize) {
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
    /* Read the length of the original memory block */
    size_t len = *plen;
    /* The actual payload size of the old block */
    size_t old_size = len - sizeof(size_t);
    /* Allocate new memory */
    void* newptr = nu_malloc (size);
    /* Check if allocation was successful */
    if (newptr == NULL) {
        return NULL;
    }
    /* Copy data from the original memory block to the new one
    Use the minimum of the new size and the old size to ensure no memory overflow */
    size_t min_size = (size < old_size) ? size : old_size;
    memcpy (newptr, ptr, min_size);
    /* Free the original memory block */
    nu_free (ptr);
    /* Return a pointer to the new memory block */
    return newptr;
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
    size_t len = *plen;
    /* Free the memory using munmap */
    munmap((void*)plen, len);
}
