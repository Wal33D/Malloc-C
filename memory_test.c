#include <stdio.h>
#include <stddef.h>
#include <errno.h>
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
        nu_free(arr); /* arr already freed on failure? but we call anyway */
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

    /* Ensure nu_free(NULL) is safe (should do nothing) */
    nu_free(NULL);

#ifdef _POSIX_VERSION
    /* Test failure paths with a huge allocation to trigger mmap error */
    size_t huge = SIZE_MAX / 2; /* size large enough to fail */
    errno = 0;
    void *too_big = nu_malloc(huge);
    if (too_big != NULL) {
        fprintf(stderr, "nu_malloc unexpectedly succeeded on huge allocation\n");
        nu_free(too_big);
        return 1;
    }
    if (errno != ENOMEM) {
        fprintf(stderr, "nu_malloc did not set ENOMEM on failure\n");
        return 1;
    }

    int *temp = (int *)nu_malloc(sizeof(int));
    if (!temp) {
        perror("nu_malloc for realloc test");
        return 1;
    }
    errno = 0;
    int *temp2 = (int *)nu_realloc(temp, huge);
    if (temp2 != NULL) {
        fprintf(stderr, "nu_realloc unexpectedly succeeded on huge allocation\n");
        nu_free(temp2);
        return 1;
    }
    if (errno != ENOMEM) {
        fprintf(stderr, "nu_realloc did not set ENOMEM on failure\n");
        nu_free(temp);
        return 1;
    }
    /* original pointer should still be valid */
    nu_free(temp);
#endif

    printf("All tests passed\n");
    return 0;
}
