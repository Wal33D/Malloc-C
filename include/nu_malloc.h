/* MIT License - see LICENSE file for details */
#ifndef NU_MALLOC_H
#define NU_MALLOC_H

// Included for the `size_t` type.
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Allocate a block of memory.
 *
 * This function behaves like the standard `malloc` and allocates a
 * block of `size` bytes. The returned memory is uninitialised.
 *
 * @param size Number of bytes to allocate.
 * @return Pointer to the allocated memory or `NULL` on failure.
 */
void* nu_malloc(size_t size);

/**
 * @brief Allocate an array and set all bytes to zero.
 *
 * The function allocates space for an array of `nmemb` elements of
 * `size` bytes each and initialises all bytes in the allocated storage
 * to zero.
 *
 * @param nmemb Number of elements to allocate.
 * @param size  Size of each element.
 * @return Pointer to the allocated memory or `NULL` on failure.
 */
void* nu_calloc(size_t nmemb, size_t size);

/**
 * @brief Resize a previously allocated memory block.
 *
 * The memory block pointed to by `ptr` is resized to `size` bytes. The
 * contents are preserved up to the minimum of the old and new sizes.
 *
 * @param ptr  Pointer to memory obtained from nu_malloc or nu_calloc.
 *             `NULL` behaves like a call to nu_malloc.
 * @param size New size in bytes. A value of `0` frees the memory.
 * @return Pointer to the reallocated memory or `NULL` on failure. When
 *         this function fails the original block is left untouched.
 */
void* nu_realloc(void* ptr, size_t size);

/**
 * @brief Free a memory block allocated with nu_malloc, nu_calloc or
 *        nu_realloc.
 *
 * Passing `NULL` is safe and has no effect.
 *
 * @param ptr Pointer to the memory block to free. May be `NULL`.
 */
void nu_free(void* ptr);

#ifdef __cplusplus
}
#endif

#endif
