#include <stdio.h>
#include "nu_malloc.h"

int main(void) {
    int *number = (int*) nu_malloc(sizeof(int));
    if (!number) {
        perror("nu_malloc");
        return 1;
    }
    *number = 42;
    printf("Value: %d\n", *number);
    nu_free(number);
    return 0;
}
