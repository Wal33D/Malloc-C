/* MIT License - see LICENSE file for details */
#include <stdio.h>
#include <stddef.h>
#include <limits.h>
#include <stdint.h>
#include <stdalign.h>
#include <errno.h>
#ifdef _POSIX_VERSION
#include <sys/resource.h>
#endif
#include "nu_malloc.h"

int main(void) {
    /* Test nu_calloc zero initialization */
    size_t n = 5;
    int *arr = (int *)nu_calloc(n, sizeof(int));
    if (!arr) {
        perror("nu_calloc");
        return 1;
    }
    for (size_t i = 0; i < n; i++) {
        if (arr[i] != 0) {
            fprintf(stderr, "nu_calloc did not zero memory\n");
            nu_free(arr);
            return 1;
        }
    }

    /* Fill array with values and test nu_realloc growth */
    for (size_t i = 0; i < n; i++) {
        arr[i] = (int)(i + 1);
    }
    size_t bigger_n = n * 2;
    int *bigger = (int *)nu_realloc(arr, bigger_n * sizeof(int));
    if (!bigger) {
        perror("nu_realloc");
        /* nu_realloc does not free the original block on failure */
        nu_free(arr);
        return 1;
    }
    for (size_t i = 0; i < n; i++) {
        if (bigger[i] != (int)(i + 1)) {
            fprintf(stderr, "nu_realloc did not preserve data on growth\n");
            nu_free(bigger);
            return 1;
        }
    }

    /* Test nu_realloc shrink */
    size_t smaller_n = n - 2; /* 3 */
    int *smaller = (int *)nu_realloc(bigger, smaller_n * sizeof(int));
    if (!smaller) {
        perror("nu_realloc");
        return 1;
    }
    for (size_t i = 0; i < smaller_n; i++) {
        if (smaller[i] != (int)(i + 1)) {
            fprintf(stderr, "nu_realloc did not preserve data on shrink\n");
            nu_free(smaller);
            return 1;
        }
    }
    nu_free(smaller);

    /* Test nu_malloc directly */
    int *value = (int *)nu_malloc(sizeof(int));
    if (!value) {
        perror("nu_malloc");
        return 1;
    }
    if (((uintptr_t)value % alignof(max_align_t)) != 0) {
        fprintf(stderr, "nu_malloc returned misaligned pointer\n");
        nu_free(value);
        return 1;
    }
    *value = 7;
    if (*value != 7) {
        fprintf(stderr, "nu_malloc memory corrupt\n");
        nu_free(value);
        return 1;
    }
    nu_free(value);

    /* Test nu_realloc with NULL (acts like malloc) */
    int *ptr = (int *)nu_realloc(NULL, sizeof(int));
    if (!ptr) {
        perror("nu_realloc NULL");
        return 1;
    }
    *ptr = 123;
    if (*ptr != 123) {
        fprintf(stderr, "nu_realloc(NULL) failed\n");
        nu_free(ptr);
        return 1;
    }
    nu_free(ptr);

    /* Test nu_realloc called with size 0 on an existing block */
    int *zero_ptr = (int *)nu_malloc(sizeof(int));
    if (!zero_ptr) {
        perror("nu_malloc for zero realloc");
        return 1;
    }
    *zero_ptr = 456;
    void *zero_result = nu_realloc(zero_ptr, 0);
    /* Either NULL or a pointer valid for nu_free is acceptable */
    if (zero_result != NULL) {
        nu_free(zero_result);
    }

    /* Test allocation of zero bytes */
    void *zero = nu_malloc(0);
    if (!zero) {
        perror("nu_malloc 0");
        return 1;
    }
    /* freeing should be safe even for size 0 allocations */
    nu_free(zero);

    /* Test allocation failure for extremely large size */
    errno = 0;
    void *too_big = nu_malloc(SIZE_MAX);
    if (too_big != NULL || errno != ENOMEM) {
        fprintf(stderr, "nu_malloc SIZE_MAX should fail with ENOMEM\n");
        nu_free(too_big);
        return 1;
    }

    int *small = (int *)nu_malloc(sizeof(int));
    if (!small) {
        perror("nu_malloc small");
        return 1;
    }
    errno = 0;
    void *realloc_fail = nu_realloc(small, SIZE_MAX);
    if (realloc_fail != NULL || errno != ENOMEM) {
        fprintf(stderr, "nu_realloc SIZE_MAX should fail with ENOMEM\n");
        nu_free(realloc_fail);
        nu_free(small);
        return 1;
    }
    nu_free(small);

#ifdef _POSIX_VERSION
    /* Simulate mmap failure by limiting address space */
    struct rlimit orig_lim;
    if (getrlimit(RLIMIT_AS, &orig_lim) != 0) {
        perror("getrlimit");
        return 1;
    }
    struct rlimit low_lim = {20 * 1024 * 1024, orig_lim.rlim_max};
    if (setrlimit(RLIMIT_AS, &low_lim) != 0) {
        perror("setrlimit");
        return 1;
    }

    errno = 0;
    void *mmap_fail = nu_malloc(64 * 1024 * 1024);
    if (mmap_fail != NULL || errno != ENOMEM) {
        fprintf(stderr, "nu_malloc RLIMIT_AS should fail with ENOMEM\n");
        nu_free(mmap_fail);
        return 1;
    }

    void *tmp = nu_malloc(1024);
    if (!tmp) {
        perror("nu_malloc tmp");
        return 1;
    }
    errno = 0;
    void *grow_fail = nu_realloc(tmp, 64 * 1024 * 1024);
    if (grow_fail != NULL || errno != ENOMEM) {
        fprintf(stderr, "nu_realloc RLIMIT_AS should fail with ENOMEM\n");
        nu_free(tmp);
        nu_free(grow_fail);
        return 1;
    }
    nu_free(tmp);
    /* restore original limit */
    setrlimit(RLIMIT_AS, &orig_lim);
#endif

    /* Ensure nu_free(NULL) is safe (should do nothing) */
    nu_free(NULL);

    printf("All tests passed\n");
    return 0;
}
