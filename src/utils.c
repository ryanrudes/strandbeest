#include <stdio.h>
#include <stdlib.h>

void check_memory(void *ptr) {
    if (ptr == NULL) {
        perror("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
}